#ifndef __USART_H__
#define __USART_H__

#include "stm32f4xx.h"
#include <stdio.h>
#include <stdbool.h>
#include "led.h" 
#include "flash.h"

#define USART_DEBUG		USART1
#define U1_DATA_LEN  256

extern volatile char usart1_recv_data[U1_DATA_LEN];
extern volatile bool usart1_recv_flag;

extern volatile uint16_t usart2_recv_data;



extern int fputc(int ch, FILE *f);

extern void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);
extern void USART3_init(u32 bound);



extern void USART1_init(uint32_t baud);
extern void USART_Send_Char(USART_TypeDef* USARTx, char ch);

extern void USART_Send_data(USART_TypeDef* USARTx,char *ch);

extern void usart1_ctr_led(void);

extern void USART2_Config(uint32_t baud);

extern void usart2_ctr_led(void);




//extern void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);

#endif
