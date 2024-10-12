#ifndef __FLASH_H__
#define __FLASH_H__

#include "stm32f4xx.h"
#include "usart.h"
#include "led.h"
#include "dht11.h"
#include "key.h"
#include "rtc.h"
#include "tim.h"
#define  MSG_LEN  84

extern void FLASH_Init(uint16_t num);

extern void FLASH_Write(uint16_t num, uint16_t offset, uint8_t *data, uint16_t data_len);
extern void FLASH_Read(uint16_t num, uint16_t offset, uint8_t *data, uint16_t data_len);
extern void flash_read_data(void);

#endif
