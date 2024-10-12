#include "spi.h"

// 初始化 W25Qxx 闪存
void W25Qxx_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;  
	
	// 使能 GPIOB 的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	// 配置 MISO 引脚为输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	// 配置 SCK、MOSI 和 CS 引脚为输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5| GPIO_Pin_14;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	// 初始化 CS 引脚为高电平
	W25Qxx_CS = 1;
	
	// 设置空闲时钟为高电平（模式3）
	W25Qxx_SCK = 1;
	
	// 设置 MOSI 电平（可以随意）
	W25Qxx_MOSI = 1;
}



// 硬件SPI3
void SPI3_W25Qxx_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	SPI_InitTypeDef  SPI_InitStructure;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/*!< Enable the SPI clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	
	/*!< SPI pins configuration *************************************************/
	/*!< Connect SPI pins to AF6 */  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);
	
	/*!< SPI SCK  MOSI MISO pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	/*!< Configure sFLASH Card CS pin in output pushpull mode ********************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	/*!< Deselect the FLASH: Chip Select high */
	W25Qxx_CS = 1;

	/*!< SPI configuration  模式0 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI通信模式：全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						//设置SPI角色：主机
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//设置数据帧长度：8bit
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							//时钟极性：空闲时为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//时钟相位：在第一个跳变沿采集数据
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//设置片选引脚控制元：软件代码控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  //设置SPI的时钟分频：速率 42MHz/8 = 5.25MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//数据传输时的第一个bit：最高有效位
	SPI_Init(SPI3, &SPI_InitStructure);

	/*!< Enable the sFLASH_SPI  */
	SPI_Cmd(SPI3, ENABLE);

}

/* 模式3：决定的是空闲时时钟信号的极性及主机收发数据的次序（相位） */
uint8_t SPI3_TransmissionByte(uint8_t send_data)
{
	 /*!< Loop while DR register in not empty */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
	
	/*!< Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI3, send_data);
	
	/*!< Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
	
	/*!< Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI3);

}



// SPI 传输一个字节
uint8_t SPI_TransmissionByte(uint8_t send_data)
{
	uint8_t recv_data=0; // 用于接收数据
	for(uint8_t i=0; i<8; i++)
	{
		// 准备发送数据
		if(send_data & (1<<(7-i)))
			W25Qxx_MOSI = 1; // 发送高电平
		else
			W25Qxx_MOSI = 0; // 发送低电平
		
		// 产生时钟前沿
		W25Qxx_SCK = 0;
		delay_us(1);
		
		// 产生时钟后沿
		W25Qxx_SCK = 1;
		delay_us(1);
		
		// 读取 MISO 数据
		if(W25Qxx_MISO)
			recv_data |= 1<<(7-i); // 保存接收到的位
	}
	
	return recv_data; // 返回接收到的数据
}

// 读取 W25Qxx 的厂商和设备 ID
uint16_t  W25Qxx_Read_ID(void)
{
	uint16_t id=0;
	
	// CS 为低电平：从机工作
	W25Qxx_CS = 0;
	
	// 发送读取 ID 的命令
	SPI_TransmissionByte(0x90);
	
	// 发送 24 位地址（无关紧要）
	SPI_TransmissionByte(0x00);
	SPI_TransmissionByte(0x00);
	SPI_TransmissionByte(0x00);
	
	// 读取厂商 ID 和设备 ID
	id = SPI_TransmissionByte(0xFF) << 8; // 读取厂商 ID
	id |= SPI_TransmissionByte(0xFF); // 读取设备 ID
	
	// CS 为高电平：从机不工作
	W25Qxx_CS = 1;
	
	return id; // 返回 ID
}

// 读取指定地址的数据
void W25Qxx_ReadData(uint32_t addr, uint8_t *p, uint32_t len)
{
	// CS 为低电平：从机工作
	W25Qxx_CS = 0;
	
	// 发送读取数据的命令
	SPI_TransmissionByte(0x03);
	
	// 发送 24 位地址
	SPI_TransmissionByte((addr>>16)&0xFF);
	SPI_TransmissionByte((addr>>8)&0xFF);
	SPI_TransmissionByte(addr&0xFF);
	
	// 读取数据
	while(len--)
	{
		*p = SPI_TransmissionByte(0xFF); // 发送填充数据以读取
		p++;
	}
	
	// CS 为高电平：从机不工作
	W25Qxx_CS = 1;
}

// 解锁闪存以允许写操作
void W25Qxx_unlock(void)
{
	// CS 为低电平
	W25Qxx_CS = 0;
	
	// 发送写使能命令
	SPI_TransmissionByte(0x06);
	
	// CS 为高电平
	W25Qxx_CS = 1;
}

// 锁定闪存以禁止写操作
void W25Qxx_lock(void)
{
	// CS 为低电平
	W25Qxx_CS = 0;
	
	// 发送写禁止命令
	SPI_TransmissionByte(0x04);
	
	// CS 为高电平
	W25Qxx_CS = 1;
}

// 读取状态寄存器1
uint8_t W25Qxx_Read_status1(void)
{
	uint8_t status;
	
	// CS 为低电平
	W25Qxx_CS = 0;
	
	// 发送读取状态寄存器的命令
	SPI_TransmissionByte(0x05);
	
	// 接收状态寄存器1的值
	status = SPI_TransmissionByte(0xFF);
	
	// CS 为高电平
	W25Qxx_CS = 1;
	
	return status; // 返回状态值
}

// 擦除指定扇区
void W25Qxx_Erase_Sector(uint32_t addr)
{
	// 解锁闪存
	W25Qxx_unlock();
	// 延时以让从机稳定
	delay_us(10);
	
	// 擦除操作
	W25Qxx_CS = 0; // CS 低电平
	SPI_TransmissionByte(0x20); // 发送擦除命令
	
	// 发送地址
	SPI_TransmissionByte((addr>>16)&0xFF);
	SPI_TransmissionByte((addr>>8)&0xFF);
	SPI_TransmissionByte(addr&0xFF);
	
	W25Qxx_CS = 1; // CS 高电平
	
	// 等待擦除完成
	while(1)
	{
		if((W25Qxx_Read_status1() & 0x01) == 0) // 检查状态
			break;
		delay_ms(1); // 延时
	}
	
	// 锁定闪存
	W25Qxx_lock();
}

// 向指定地址编程
void W25Qxx_Page_Program(uint32_t addr, uint8_t *p, uint32_t len)
{
	// 一次编程最多 256 字节
	uint8_t status;
	
	// 判断是否需要擦除
	//.....  // 此处可以添加判断逻辑
	
	// 解锁
	W25Qxx_unlock();
	// 延时以让从机识别指令
	delay_us(10);
	
	// CS 低电平开始编程
	W25Qxx_CS = 0;
	
	// 发送写命令
	SPI_TransmissionByte(0x02);
	
	// 发送地址
	SPI_TransmissionByte((addr>>16) & 0xFF);
	SPI_TransmissionByte((addr>>8) & 0xFF);
	SPI_TransmissionByte(addr & 0xFF);
	
	// 发送数据
	while(len--)
	{
		SPI_TransmissionByte(*p);
		p++;
	}
	
	// 等待编程完成
	while(1)
	{
		status = W25Qxx_Read_status1(); // 读取状态
		if((status & 0x01) == 0) // 检查状态
			break;
		
		delay_ms(1); // 延时
	}
	
	// CS 高电平，结束操作
	W25Qxx_CS = 1;
	
	// 锁定闪存
	W25Qxx_lock();
}
