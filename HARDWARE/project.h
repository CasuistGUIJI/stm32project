#ifndef __PROJECT_H__
#define __PROJECT_H__

#include "stm32f4xx.h"
#include <string.h>
#include <stdlib.h>
#include "usart.h"
#include "key.h"
#include "dht11.h"
#include "rtc.h"
#include "flash.h"
#include "oled.h"

#define  START  1
#define  STOP   2
#define  SHOW   3
#define  CLEAR  4

extern uint8_t cmd;
extern uint16_t temp, humi;
extern uint32_t msg_num;

extern void record_dht11(void);
	
#endif



//   主函数所需：
//  // 明确中断优先级分组：分组2
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	// 初始化串口1
//	USART1_init(115200);
//	if (RTC_ReadBackupRegister(RTC_BKP_DR3) != 0x7777)
//	{
//		RTC_INITConfig();
//		/* 将一个数据写入备份寄存器的0号寄存器中，用于初始化RTC的标记 */
//		RTC_WriteBackupRegister(RTC_BKP_DR3, 0x7777);
//	}
//	RTC_WakeUp_Config();
//	KEY_init();
//	EXTI_Config();
//	DHT11_INIT();
//	dth11_read_data(&temp, &humi);
//	// 初始化编号 
//	FLASH_Read(4,0, (uint8_t *)&msg_num, sizeof msg_num);
//	printf("this is DHT11 msg\r\n");
