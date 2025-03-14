//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : Evk123
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : 0.69寸OLED 接口演示例程(STM32F103ZE系列IIC)
//              说明: 
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PD6（SCL）
//              SDA   接PD7（SDA）            
//              ----------------------------------------------------------------
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
#ifndef __OLED_H
#define __OLED_H			  	 
#include "bit_band.h"
#include "stdlib.h"	
#include "dht11.h"	


#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED IIC端口定义----------------  					   

#define  IIC_SCL   	 PBout(15)
#define  IIC_SDA_O   PDout(10)
#define  IIC_SDA_I   PDin(10)
 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);
void fill_picture(unsigned char fill_Data);
void Picture(void);
void IIC_Start(void);
void IIC_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);

void IIC_Wait_Ack(void);

void SDA_OLED_Mode(GPIOMode_TypeDef mode);

void OLED_Show_dht11(void);
void OLED_Data_rtc(void);

void OLED_Read_DATA_time(void);


u8 b_h(u8 a);

void showtime(void);

void show_start(void);

void Display_On_LED_Screen(int temp, int humi);

#endif  
	 
//	while(1)
//	{
//		Read_DHT11(&DHT11_Data);
//		// 显示年/月/日/星期 及实时时间
//	    showtime();
////		//温湿度数据
//		OLED_Show_dht11(); 
//		if(DHT11_Data.humi_int >= 36 && DHT11_Data.temp_int >= 29)
//		{
//			PFout(8) = 1;
//			PEout(13) = 0;
//			PEout(14) = 0;
//			delay_ms(100);
//			
//			PFout(8) = 0;
//			PEout(13) = 1;
//			PEout(14) = 1;
//			delay_ms(200);
//			
//			PFout(8) = 1;
//			PEout(13) = 0;
//			PEout(14) = 0;
//			delay_ms(100);
//			
//			PFout(8) = 0;
//			PEout(13) = 1;
//			PEout(14) = 1;
//			delay_ms(200);
//		}else if(DHT11_Data.temp_int >= 30)
//		{
//			
//			PFout(8) = 1;
//			PEout(13) = 0;
//			delay_ms(250);

//			PFout(8) = 0;
//			PEout(13) = 1;
//			delay_ms(250);
//		}else if(DHT11_Data.humi_int >= 40)
//		{
//			PFout(8) = 1;
//			PEout(14) = 0;
//			delay_ms(200);

//			PFout(8) = 0;
//			PEout(14) = 1;
//			delay_ms(200);

//		}
////		record_dht11();
//		
//		delay_ms(200);
//	}
//	


