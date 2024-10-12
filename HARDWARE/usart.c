#include "usart.h"
 
struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

volatile char usart1_recv_data[U1_DATA_LEN];
volatile bool usart1_recv_flag=false;

volatile uint16_t usart2_recv_data;

int fputc(int ch, FILE *f)
{
    USART_Send_Char(USART1, ch);
    return ch;
}


//void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...)
//{

//	unsigned char UsartPrintfBuf[296];
//	va_list ap;
//	unsigned char *pStr = UsartPrintfBuf;
//	
//	va_start(ap, fmt);
//	vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap);							//格式化
//	va_end(ap);
//	
//	while(*pStr != 0)
//	{
//		USART_SendData(USARTx, *pStr++);
//		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
//	}

//}


void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{

	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//发送数据
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//等待发送完成
	}

}

void USART3_init(u32 bound)
{
    USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);


	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART1);

	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    //上拉
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    //复用功能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //高速
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	
	USART_InitStructure.USART_BaudRate = bound;   //通信双方必须保持一致
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART3, &USART_InitStructure);
	
	/* 配置NVIC */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;               //指定中断请求的通道：串口1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;   //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		   //响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				   //指定中断请求的通道状态
	NVIC_Init(&NVIC_InitStructure);
	
	/* 开启串口接收中断 */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	/* Enable USART3 */
	USART_Cmd(USART3, ENABLE);
	printf("USART3_INIT_Success\r\n");
}


void USART1_init(uint32_t baud)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable GPIOA clock：使能GPIOA时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/* Enable UART clock：使能UART1时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* Connect PA9 to USART1_Tx*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	/* Connect PA10 to USART1_Rx*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	/* Configure USART Tx an Rx as alternate function  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  // 推挽
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    // 上拉
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    // 复用功能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;  // 9、10引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;// 速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 configuration ------------------------------------------------------*/
	/* USART1 configured as follows:
		- BaudRate = baud  
		- Word Length = 8 Bits
		- One Stop Bit
		- No parity
		- Hardware flow control disabled (RTS and CTS signals)
		- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = baud;				    //波特率：baud
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;      //停止位：1位
	USART_InitStructure.USART_Parity = USART_Parity_No;			//校验位：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //不使用硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收和发送模式
	/* USART1 configuration */
	USART_Init(USART1, &USART_InitStructure);
	
	/* Enable USART */
	USART_Cmd(USART1, ENABLE);
	
	/* 清除发送完成标志 */
	USART_ClearFlag(USART1,USART_FLAG_TC);
	
	/* 配置NVIC */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;             // 指定配置中断通道：串口1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    // 指定中断通道的抢占优先级：最高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           // 指定中断通道的响应优先级：最高
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 // 指定中断通道触发信号状态：使能
	NVIC_Init(&NVIC_InitStructure); // 按照结构体的初始化去配置硬件
	
	// 清除接收中断标志
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	
	// 使能串口1接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	printf("USART1_INIT_Success\r\n");
	
}

void USART2_Config(uint32_t baud)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Peripheral Clock Enable -------------------------------------------------*/
    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /* Enable USART clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* USARTx GPIO configuration -----------------------------------------------*/ 
    /* Connect USART pins to AF7 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

    /* Configure USART Tx and Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  // USART2 TX
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; // USART2 RX
    GPIO_Init(GPIOA, &GPIO_InitStructure);    

    /* USARTx configuration ----------------------------------------------------*/
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART2, &USART_InitStructure);

	/* Enable USART */
    USART_Cmd(USART2, ENABLE);
    /* Clear any pending flags */
    USART_ClearFlag(USART2, USART_FLAG_TC);
    
    /* Configure NVIC for USART1 */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* Enable USART1 RX interrupt */
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  
}


void USART_Send_Char(USART_TypeDef* USARTx, char ch)
{
    // 等待 USART 数据寄存器空
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    // 将字符放入数据寄存器
    USART_SendData(USARTx, (uint8_t)ch);
    // 等待数据发送完成
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
}

void USART_Send_data(USART_TypeDef* USARTx, char *ch)
{
    while (*ch)
    {
        USART_Send_Char(USARTx, *ch);
        ch++;
    }
}


void USART1_IRQHandler(void) {
    static uint16_t recv_count = 0;

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        char received_char = USART_ReceiveData(USART1);

        // 检查 recv_count 是否超过缓冲区大小
        if (recv_count < U1_DATA_LEN - 1) {
            usart1_recv_data[recv_count++] = received_char;
            
            if (received_char == '#') {
                usart1_recv_data[recv_count - 1] = '\0';  // 结束符
                recv_count = 0;  // 重置计数器
                usart1_recv_flag = true;  // 设置接收完成标志
            }
        } else {
            // 缓冲区溢出处理
            recv_count = 0;
        }

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);  // 清除中断标志
    }
}





void usart1_ctr_led(void)
{
//	switch (usart1_recv_data)
//	{
//	case 0x00:
//		PFout(9) = 0;
//		break;
//	case 0x01:
//		PFout(10) = 0;
//		break;
//	case 0x02:
//		PEout(13) = 0;
//		break;
//	case 0x03:
//		PEout(14) = 0;
//		break;
//	case 0x10:
//		PFout(9) = 1;
//		break;
//	case 0x20:
//		PFout(10) = 1;
//		break;
//	case 0x30:
//		PEout(13) = 1;
//		break;
//	case 0x40:
//		PEout(14) = 1;
//		break;
//	default:
//		break;
//	}
}
void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        usart2_recv_data = USART_ReceiveData(USART2);
 //      printf("%#02x",usart2_recdata);
		 printf("%c",usart2_recv_data);
		 USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}

void usart2_ctr_led(void)
{
	switch (usart2_recv_data)
	{
	case '0':
		usart2_recv_data = 0;
		PFout(9) = 0;
		break;
	case '1':
		usart2_recv_data = 0;
		PFout(10) = 0;
		break;
	case '2':
		PEout(13) = 0;
		break;
	case '3':
		PEout(14) = 0;
		break;
	case 'a':
		PFout(9) = 1;
		break;
	case 'b':
		PFout(10) = 1;
		break;
	case 'c':
		PEout(13) = 1;
		break;
	case 'd':
		PEout(14) = 1;
		break;
	default:
		break;
	}
}
