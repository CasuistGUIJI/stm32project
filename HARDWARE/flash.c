#include "flash.h"

#define MAX_RECORDS 100
#define FLASH_START_ADDR 0x08010000 // 存储开始的 Flash 地址
//uint16_t temperature, humidity;
uint8_t record_count = 0;  // 当前记录数
bool record_flag = false;    // 是否处于记录状态

uint8_t buff[64] = {0};

uint32_t Page[8] = {FLASH_Sector_0,FLASH_Sector_1,FLASH_Sector_2,FLASH_Sector_3,FLASH_Sector_4,FLASH_Sector_5,FLASH_Sector_6,FLASH_Sector_7};
uint32_t Address[8] = {0x08000000, 0x08004000, 0x08008000, 0x0800C000, 0x08010000,0x08020000,0x08040000,0x08060000}; 

void FLASH_Init(uint16_t num)
{
	/* 解锁Flash */
	FLASH_Unlock();
	
	/* 清除相关标志位 */  
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
					FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR); 

	/* 擦除扇区，擦除电压为2.7v-3.6v：按字擦除 */ 
	if (FLASH_EraseSector(Page[num], VoltageRange_3) != FLASH_COMPLETE)
	{ 
		printf("擦除失败\r\n");
		while(1)
		{}
	}
	/* 锁定  */
	FLASH_Lock(); 
}

/* data_len:需要满足整除4 */
void FLASH_Write(uint16_t num, uint16_t offset, uint8_t *data, uint16_t data_len)
{
	uint32_t addr = Address[num]+offset;
	uint32_t tmp;
	/* 检查扇区是否需要擦除 */
	if(*((volatile uint32_t *)addr) != 0xFFFFFFFF)
	{
		/* 擦除扇区 */
		FLASH_Init(num);
	}
	
	/* 解锁Flash */
	FLASH_Unlock();
	
	/* 编程 */
	for(int i=0; i<data_len/4; i++)
	{
		/* 一次写一个字 */
		tmp = (*data | (*(data+1)<<8) | *(data+2)<<16 | *(data+3) <<24);
		if (FLASH_ProgramWord(addr, tmp) != FLASH_COMPLETE)
		{
			printf("编程失败\r\n");
			return;
		}
		
		data += 4;
		addr += 4;
	}
	
	/* 锁定  */
	FLASH_Lock(); 
}

void FLASH_Read(uint16_t num, uint16_t offset, uint8_t *data, uint16_t data_len)
{
	// 需要读取FLASH数据的地址
	volatile uint8_t *addr = (volatile uint8_t *)Address[num]+offset;
	
	for(int i=0; i<data_len; i++)
	{
		if(((*addr) | (*(addr+1))<<8 | (*(addr+2))<<16 | (*(addr+3))<<24) == 0xFFFFFFFF)
		{
			printf("无效数据\r\n");
			break;
		}
		
		*(data+i) = *(addr+i);	// data[i] = addr[i];
	}
}


void flash_read_data(void)
{
    DHT11_Data_TypeDef DHT11_Data;
    static int num =0;
    // 读取温湿度数据
    if(Read_DHT11(&DHT11_Data)==SUCCESS)
    {
        // 定义用于存储数据的缓冲区
        uint8_t buf[64] = {0}; // 初始化为0

        // 使用 sprintf 将温湿度数据格式化为字符串并存入 buf
        sprintf((char *)buf, "[%d] T:%d°C H:%d%% \r\n", num++,DHT11_Data.temp_int, DHT11_Data.humi_int);
        
        // 将格式化后的数据写入 flash
        FLASH_Write(7, 0, buf, 64);

        // 定义一个用于读取 flash 数据的缓冲区
        uint8_t data[64] = {0};

        // 从 flash 中读取数据
        FLASH_Read(7, 0, (uint8_t *)data, 64);

        // 打印从 flash 中读取的数据
        printf("%s", (char *)data);
    }
    else
    {
        // 如果读取温湿度失败，打印错误信息
        printf("Failed to read temperature and humidity data.\r\n");
    }
}

