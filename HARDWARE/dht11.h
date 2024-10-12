﻿#ifndef __DHT11_H
#define	__DHT11_H

#include "stm32f4xx.h"
#include "bit_band.h"
#include "delay.h"

#define HIGH  1
#define LOW   0

typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
		                 
}DHT11_Data_TypeDef;
//IO方向设置
#define DHT11_IO_IN()  {GPIOG->CRH&=0XFFF0FFFF;GPIOG->CRH|=8<<16;}
#define DHT11_IO_OUT() {GPIOG->CRH&=0XFFF0FFFF;GPIOG->CRH|=3<<16;}
////IO操作函数											   
#define	DHT11_DQ_OUT PGout(9) //数据端口	PA0 
#define	DHT11_DQ_IN  PGin(9)  //数据端口	PA0 

u8 DHT11_Init(void);//初始化DHT11
void DHT11_Rst(void);//复位DHT11 
u8 DHT11_Read_Byte(void);//读出一个字节
u8 DHT11_Read_Bit(void);//读出一个位
u8 DHT11_Check(void);//检测是否存在DHT11

uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data);

#endif /* __DHT11_H */







