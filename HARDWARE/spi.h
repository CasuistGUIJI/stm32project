#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f4xx.h"
#include "bit_band.h"

#define  W25Qxx_CS  	PBout(14)
#define  W25Qxx_SCK  	PBout(3)
#define  W25Qxx_MISO 	PBin(4)
#define  W25Qxx_MOSI  	PBout(5)

//硬件配置SPI3
extern void SPI3_W25Qxx_Init(void);
//硬件配置SPI3传输字节
extern uint8_t SPI3_TransmissionByte(uint8_t send_data);



// 初始化 W25Qxx 闪存
extern void W25Qxx_Init(void);
// SPI 传输一个字节
extern uint8_t SPI_TransmissionByte(uint8_t send_data);
// 读取 W25Qxx 的厂商和设备 ID
extern uint16_t  W25Qxx_Read_ID(void);
// 读取指定地址的数据
extern void W25Qxx_ReadData(uint32_t addr, uint8_t *p, uint32_t len);
// 解锁闪存以允许写操作
extern void W25Qxx_unlock(void);
// 锁定闪存以禁止写操作
extern void W25Qxx_lock(void);
// 读取状态寄存器1
extern uint8_t W25Qxx_Read_status1(void);
// 擦除指定扇区
extern void W25Qxx_Erase_Sector(uint32_t addr);
// 向指定地址编程
extern void W25Qxx_Page_Program(uint32_t addr, uint8_t *p, uint32_t len);
	
#endif



//主函数

//int main(void)
//{
//	uint16_t id;
//	uint8_t buf[64];
//	
//	systick_init(1);
//	
//	// 初始化串口1
//	USART1_init(115200);
//	
//	printf("This is W25Q128 read id test\r\n");
//	
//	W25Qxx_Init();
//	
//	id = W25Qxx_Read_ID();
//	
//	printf("厂商ID:%#X 设备ID:%#X\r\n", id>>8, id&0XFF);
//	
//	W25Qxx_Erase_Sector(0x2000); 
//	
//	u8 data[5] = {0x12,0x34,0x56,0x78, 0x90};
//	W25Qxx_Page_Program(0x2000, data, sizeof(data));
//	
//	W25Qxx_ReadData(0x2000, buf, sizeof buf);
//	for(int i=0; i<sizeof buf; i++)
//	{
//		printf("%#x ", buf[i]);
//	}
//	printf("\r\n");
//	TIM14_Config();
//	ADC_photosensitive_Config();
//	
//	while (1)
//	{
//		ADC_convert(ADC3,12);
//	}	
//	
//}