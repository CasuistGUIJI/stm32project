#include "oled.h"
#include "oledfont.h"  	 
#include "delay.h"
#include "bmp.h"
#include "usart.h"


//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
/**********************************************
//IIC Start
**********************************************/
/**********************************************
//IIC Start
**********************************************/
volatile u8 time_buf[8] = {0};


void OLED_Read_DATA_time(void)  
{
	RTC_TimeTypeDef RTC_TimeStructure;  // RTC时间结构体
	RTC_DateTypeDef RTC_DateStructure;  // RTC日期结构体
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

	time_buf[1]=RTC_DateStructure.RTC_Year;		//年 
	time_buf[2]=RTC_DateStructure.RTC_Month;		//月 
	time_buf[3]=RTC_DateStructure.RTC_Date;		//日 

	time_buf[4]=RTC_TimeStructure.RTC_Hours;		//时 
	time_buf[5]=RTC_TimeStructure.RTC_Minutes;		//分 
	time_buf[6]=RTC_TimeStructure.RTC_Seconds;//秒，屏蔽秒的第7位，避免超出59
	time_buf[7]=RTC_DateStructure.RTC_WeekDay;		//周 	
}
u8 b_h(u8 a)
{
	u8 b;
  	   switch(a)
	   {
	   		case 0x00 :  b=0;break;
			case 0x01 :  b=1;break;
			case 0x02 :  b=2;break;
			case 0x03 :  b=3;break;
			case 0x04 :  b=4;break;
			case 0x05 :  b=5;break;
			case 0x06 :  b=6;break;
			case 0x07 :  b=7;break;
			case 0x08 :  b=8;break;
			case 0x09 :  b=9;break;
			case 0x10 :  b=10;break;
			case 0x11 :  b=11;break;
			case 0x12 :  b=12;break;
			case 0x13 :  b=13;break;
			case 0x14 :  b=14;break;
			case 0x15 :  b=15;break;
			case 0x16 :  b=16;break;
			case 0x17 :  b=17;break;
			case 0x18 :  b=18;break;
			case 0x19 :  b=19;break;
			case 0x20 :  b=20;break;
			case 0x21 :  b=21;break;
			case 0x22 :  b=22;break;
			case 0x23 :  b=23;break;
			case 0x24 :  b=24;break;
			case 0x25 :  b=25;break;
			case 0x26 :  b=26;break;
			case 0x27 :  b=27;break;
			case 0x28 :  b=28;break;
			case 0x29 :  b=29;break;
			case 0x30 :  b=30;break;
			case 0x31 :  b=31;break;
			case 0x32 :  b=32;break;
			case 0x33 :  b=33;break;
			case 0x34 :  b=34;break;
			case 0x35 :  b=35;break;
			case 0x36 :  b=36;break;
			case 0x37 :  b=37;break;
			case 0x38 :  b=38;break;
			case 0x39 :  b=39;break;
			case 0x40 :  b=40;break;
			case 0x41 :  b=41;break;
			case 0x42 :  b=42;break;
			case 0x43 :  b=43;break;
			case 0x44 :  b=44;break;
			case 0x45 :  b=45;break;
			case 0x46 :  b=46;break;
			case 0x47 :  b=47;break;
			case 0x48 :  b=48;break;
			case 0x49 :  b=49;break;
			case 0x50 :  b=50;break;
			case 0x51 :  b=51;break;
			case 0x52 :  b=52;break;
			case 0x53 :  b=53;break;
			case 0x54 :  b=54;break;
			case 0x55 :  b=55;break;
			case 0x56 :  b=56;break;
			case 0x57 :  b=57;break;
			case 0x58 :  b=58;break;
			case 0x59 :  b=59;break;
			default   : break;
	   }
	   return b;
}


void showtime(void)
{
	
	OLED_Read_DATA_time();

	OLED_ShowChar(0,0,'2',16);
	OLED_ShowChar(8,0,'0',16);
//	OLED_ShowChar(16,0,'2');
//	OLED_ShowChar(24,0,'1');
	
    switch((time_buf[1]/10))
	{
		case 0 :  	OLED_ShowChar(16,0,'0',16); break;
		case 1 :  	OLED_ShowChar(16,0,'1',16); break;
		case 2 :  	OLED_ShowChar(16,0,'2',16); break;
		case 3 :  	OLED_ShowChar(16,0,'3',16); break;
		case 4 :  	OLED_ShowChar(16,0,'4',16); break;
		case 5 :  	OLED_ShowChar(16,0,'5',16); break;
		case 6 :  	OLED_ShowChar(16,0,'6',16); break;
		case 7 :  	OLED_ShowChar(16,0,'7',16); break;
		case 8 :  	OLED_ShowChar(16,0,'8',16); break;
		case 9 :  	OLED_ShowChar(16,0,'9',16); break;
	}
	
	    switch((time_buf[1]%10))
	{
		case 0 :  	OLED_ShowChar(24,0,'0',16); break;
		case 1 :  	OLED_ShowChar(24,0,'1',16); break;
		case 2 :  	OLED_ShowChar(24,0,'2',16); break;
		case 3 :  	OLED_ShowChar(24,0,'3',16); break;
		case 4 :  	OLED_ShowChar(24,0,'4',16); break;
		case 5 :  	OLED_ShowChar(24,0,'5',16); break;
		case 6 :  	OLED_ShowChar(24,0,'6',16); break;
		case 7 :  	OLED_ShowChar(24,0,'7',16); break;
		case 8 :  	OLED_ShowChar(24,0,'8',16); break;
		case 9 :  	OLED_ShowChar(24,0,'9',16); break;
	}
	OLED_ShowChar(32,0,'-',16);

	switch((time_buf[2]/10))
	{
		case 0 :  	OLED_ShowChar(40,0,'0',16); break;
		case 1 :  	OLED_ShowChar(40,0,'1',16); break;
	}
	switch((time_buf[2]%10))
	{
		case 0 :  	OLED_ShowChar(48,0,'0',16); break;
		case 1 :  	OLED_ShowChar(48,0,'1',16); break;
		case 2 :  	OLED_ShowChar(48,0,'2',16); break;
		case 3 :  	OLED_ShowChar(48,0,'3',16); break;
		case 4 :  	OLED_ShowChar(48,0,'4',16); break;
		case 5 :  	OLED_ShowChar(48,0,'5',16); break;
		case 6 :  	OLED_ShowChar(48,0,'6',16); break;
		case 7 :  	OLED_ShowChar(48,0,'7',16); break;
		case 8 :  	OLED_ShowChar(48,0,'8',16); break;
		case 9 :  	OLED_ShowChar(48,0,'9',16); break;
	}
	OLED_ShowChar(56,0,'-',16);
	
	switch((time_buf[3]/10))
	{
		case 0 :  	OLED_ShowChar(64,0,'0',16); break;
		case 1 :  	OLED_ShowChar(64,0,'1',16); break;
		case 2 :  	OLED_ShowChar(64,0,'2',16); break;
		case 3 :  	OLED_ShowChar(64,0,'3',16); break;
	}
	switch((time_buf[3]%10))
	{
		case 0 :  	OLED_ShowChar(72,0,'0',16); break;
		case 1 :  	OLED_ShowChar(72,0,'1',16); break;
		case 2 :  	OLED_ShowChar(72,0,'2',16); break;
		case 3 :  	OLED_ShowChar(72,0,'3',16); break;
		case 4 :  	OLED_ShowChar(72,0,'4',16); break;
		case 5 :  	OLED_ShowChar(72,0,'5',16); break;
		case 6 :  	OLED_ShowChar(72,0,'6',16); break;
		case 7 :  	OLED_ShowChar(72,0,'7',16); break;
		case 8 :  	OLED_ShowChar(72,0,'8',16); break;
		case 9 :  	OLED_ShowChar(72,0,'9',16); break;
	}

//	if(b_h(time_buf[1]) != bootconst)
//	{
//		OLED_DrawBMP(0,0,128,8,BMP20);
//		jingbflag = 0;
//	}
//	else
//	{
//		jingbflag = 1;
//	}
	switch((time_buf[4]/10))
	{
		case 0 :  OLED_DrawBMP(0,2,17,6,BMP10); break;
		case 1 :  OLED_DrawBMP(0,2,17,6,BMP1); break;
		case 2 :  OLED_DrawBMP(0,2,17,6,BMP2); break;
	}
	switch((time_buf[4]%10))
	{
		case 0 :  OLED_DrawBMP(15,2,32,6,BMP10); break;
		case 1 :  OLED_DrawBMP(15,2,32,6,BMP1); break;
		case 2 :  OLED_DrawBMP(15,2,32,6,BMP2); break;
		case 3 :  OLED_DrawBMP(15,2,32,6,BMP3); break;
		case 4 :  OLED_DrawBMP(15,2,32,6,BMP4); break;
		case 5 :  OLED_DrawBMP(15,2,32,6,BMP5); break;
		case 6 :  OLED_DrawBMP(15,2,32,6,BMP6); break;
		case 7 :  OLED_DrawBMP(15,2,32,6,BMP7); break;
		case 8 :  OLED_DrawBMP(15,2,32,6,BMP8); break;
		case 9 :  OLED_DrawBMP(15,2,32,6,BMP9); break;
	}
	
	OLED_DrawBMP(30,2,47,6,BMP18);
	
	switch((time_buf[5]/10))
	{
		case 0 :  OLED_DrawBMP(47,2,64,6,BMP10); break;
		case 1 :  OLED_DrawBMP(47,2,64,6,BMP1); break;
		case 2 :  OLED_DrawBMP(47,2,64,6,BMP2); break;
		case 3 :  OLED_DrawBMP(47,2,64,6,BMP3); break;
		case 4 :  OLED_DrawBMP(47,2,64,6,BMP4); break;
		case 5 :  OLED_DrawBMP(47,2,64,6,BMP5); break;
		case 6 :  OLED_DrawBMP(47,2,64,6,BMP6); break;
		case 7 :  OLED_DrawBMP(47,2,64,6,BMP7); break;
		case 8 :  OLED_DrawBMP(47,2,64,6,BMP8); break;
		case 9 :  OLED_DrawBMP(47,2,64,6,BMP9); break;
	}
	switch((time_buf[5]%10))
	{
		case 0 :  OLED_DrawBMP(62,2,79,6,BMP10); break;
		case 1 :  OLED_DrawBMP(62,2,79,6,BMP1); break;
		case 2 :  OLED_DrawBMP(62,2,79,6,BMP2); break;
		case 3 :  OLED_DrawBMP(62,2,79,6,BMP3); break;
		case 4 :  OLED_DrawBMP(62,2,79,6,BMP4); break;
		case 5 :  OLED_DrawBMP(62,2,79,6,BMP5); break;
		case 6 :  OLED_DrawBMP(62,2,79,6,BMP6); break;
		case 7 :  OLED_DrawBMP(62,2,79,6,BMP7); break;
		case 8 :  OLED_DrawBMP(62,2,79,6,BMP8); break;
		case 9 :  OLED_DrawBMP(62,2,79,6,BMP9); break;
	}
	OLED_DrawBMP(79,2,96,6,BMP18);
	
	switch((time_buf[6]/10))
	{
		case 0 :  OLED_DrawBMP(96,2,113,6,BMP10); break;
		case 1 :  OLED_DrawBMP(96,2,113,6,BMP1); break;
		case 2 :  OLED_DrawBMP(96,2,113,6,BMP2); break;
		case 3 :  OLED_DrawBMP(96,2,113,6,BMP3); break;
		case 4 :  OLED_DrawBMP(96,2,113,6,BMP4); break;
		case 5 :  OLED_DrawBMP(96,2,113,6,BMP5); break;
		case 6 :  OLED_DrawBMP(96,2,113,6,BMP6); break;
		case 7 :  OLED_DrawBMP(96,2,113,6,BMP7); break;
		case 8 :  OLED_DrawBMP(96,2,113,6,BMP8); break;
		case 9 :  OLED_DrawBMP(96,2,113,6,BMP9); break;
	}
	switch((time_buf[6]%10))
	{
		case 0 :  OLED_DrawBMP(111,2,128,6,BMP10); break;
		case 1 :  OLED_DrawBMP(111,2,128,6,BMP1); break;
		case 2 :  OLED_DrawBMP(111,2,128,6,BMP2); break;
		case 3 :  OLED_DrawBMP(111,2,128,6,BMP3); break;
		case 4 :  OLED_DrawBMP(111,2,128,6,BMP4); break;
		case 5 :  OLED_DrawBMP(111,2,128,6,BMP5); break;
		case 6 :  OLED_DrawBMP(111,2,128,6,BMP6); break;
		case 7 :  OLED_DrawBMP(111,2,128,6,BMP7); break;
		case 8 :  OLED_DrawBMP(111,2,128,6,BMP8); break;
		case 9 :  OLED_DrawBMP(111,2,128,6,BMP9); break;
	}
	
	switch((time_buf[7]))
	{
		case 1 :  OLED_ShowCHinese(81,0,16);OLED_ShowCHinese(96,0,17);OLED_ShowCHinese(113,0,18); break;
		case 2 :  OLED_ShowCHinese(81,0,16);OLED_ShowCHinese(96,0,17);OLED_ShowCHinese(113,0,19); break;
		case 3 :  OLED_ShowCHinese(81,0,16);OLED_ShowCHinese(96,0,17);OLED_ShowCHinese(113,0,20); break;
		case 4 :  OLED_ShowCHinese(81,0,16);OLED_ShowCHinese(96,0,17);OLED_ShowCHinese(113,0,21); break;
		case 5 :  OLED_ShowCHinese(81,0,16);OLED_ShowCHinese(96,0,17);OLED_ShowCHinese(113,0,22); break;
		case 6 :  OLED_ShowCHinese(81,0,16);OLED_ShowCHinese(96,0,17);OLED_ShowCHinese(113,0,23); break;
		case 7 :  OLED_ShowCHinese(81,0,16);OLED_ShowCHinese(96,0,17);OLED_ShowCHinese(113,0,24); break;
	}

}






void OLED_Data_rtc(void)
{
	RTC_TimeTypeDef RTC_TimeStructure;  // RTC时间结构体
	RTC_DateTypeDef RTC_DateStructure;  // RTC日期结构体
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
	
	
	OLED_ShowChar(0,0,'2',16);
	OLED_ShowChar(8,0,'0',16);
	OLED_ShowNum(16,0,RTC_DateStructure.RTC_Year,2,16);//year
	OLED_ShowChar(32,0,'-',16);
	OLED_ShowNum(40,0,RTC_DateStructure.RTC_Month,1,16);//month
	OLED_ShowChar(48,0,'-',16);
	OLED_ShowNum(56,0,RTC_DateStructure.RTC_Date,2,16);//day
	OLED_ShowChar(70,0,' ',16);
	OLED_ShowCHinese(78,0,16);
	OLED_ShowCHinese(94,0,17);
	OLED_ShowNum(115,0,RTC_DateStructure.RTC_WeekDay,1,16);//day
}



void show_start(void)
{
	OLED_ShowCHinese(23,0,10);
	OLED_ShowCHinese(38,0,12);
	OLED_ShowCHinese(53,0,11);
	OLED_ShowCHinese(68,0,25);
	OLED_ShowCHinese(83,0,26);
	OLED_ShowCHinese(38,3,27);
	OLED_ShowCHinese(53,3,28);
	OLED_ShowCHinese(68,3,29);
	delay_ms(1000);
}

void Display_On_LED_Screen(int temp, int humi) {
    // 清空 OLED 显示
//    OLED_Clear();

	showtime();
    OLED_ShowCHinese(0,6,10);//显示温度汉字
	OLED_ShowCHinese(15,6,11);
	OLED_ShowChar(28,6,':',16);
	OLED_ShowNum(35,6,temp,2,16);//显示温度数值
	OLED_ShowChar(55,6,'C',16);
	
	OLED_ShowCHinese(68,6,12);//显示湿度汉字
	OLED_ShowCHinese(83,6,11);
	OLED_ShowChar(96,6,':',16);
	OLED_ShowNum(104,6,humi,2,16);//显示湿度数值
	OLED_ShowChar(121,6,'F',16);
}


void OLED_Show_dht11(void)
{
	DHT11_Data_TypeDef DHT11_Data;
	Read_DHT11(&DHT11_Data);
	
	u8 temp = DHT11_Data.temp_int;
	u8 humi = DHT11_Data.humi_int;
//	OLED_Clear();
	OLED_ShowCHinese(0,6,10);//显示温度汉字
	OLED_ShowCHinese(15,6,11);
	OLED_ShowChar(28,6,':',16);
	OLED_ShowCHinese(68,6,12);//显示湿度汉字
	OLED_ShowCHinese(83,6,11);
	OLED_ShowChar(96,6,':',16);
	OLED_ShowNum(35,6,temp,2,16);//显示温度数值
	OLED_ShowChar(55,6,'C',16);
	OLED_ShowNum(104,6,humi,2,16);//显示湿度数值
	OLED_ShowChar(121,6,'F',16);
	
	
	
}

void SDA_OLED_Mode(GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  
	GPIO_InitStructure.GPIO_Mode = mode;		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	     // 引脚外部链接了上拉电阻，可以专门提供高电平	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
}

void IIC_Start()
{

	//将SDA配置为输出模式
	SDA_OLED_Mode(GPIO_Mode_OUT);
	
	IIC_SCL = 1;
	IIC_SDA_O = 1;
	delay_us(5);
	
	IIC_SDA_O = 0;
	delay_us(5);
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
	//将SDA配置为输出模式
	SDA_OLED_Mode(GPIO_Mode_OUT);
	
	IIC_SCL = 1;
	IIC_SDA_O = 0;
	delay_us(5);
	
	IIC_SDA_O = 1;
	delay_us(5);
	
}

void IIC_Wait_Ack()
{
	//将SDA配置为输入模式
	SDA_OLED_Mode(GPIO_Mode_IN);
	
	IIC_SCL = 1;
	delay_us(5);
	
	// 将总线拉低保持占用（钳住总线）
	IIC_SCL = 0;
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
		//将SDA配置为输出模式
	SDA_OLED_Mode(GPIO_Mode_OUT);
	
	IIC_SCL = 0;
	IIC_SDA_O = 0;
	delay_us(5);
	
	for(u8 i=0; i<8; i++)
	{
		if(IIC_Byte & (1<<(7-i)))
			IIC_SDA_O = 1;
		else
			IIC_SDA_O = 0;
		delay_us(5);
		
		IIC_SCL = 1;
		delay_us(5);
		
		IIC_SCL = 0;
		delay_us(5);
	}
}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
	IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
	IIC_Stop();
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
		Write_IIC_Data(dat);
	else 
		Write_IIC_Command(dat);
}


/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
		{
			OLED_WR_Byte(fill_Data,1);
		}
	}
}


//坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
			OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
			OLED_WR_Byte(1,OLED_DATA); 
	} //更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';//得到偏移后的值			
	if(x>Max_Column-1){x=0;y=y+2;}
	if(Char_Size ==16)
	{
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
	}
	else 
	{	
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WR_Byte(F6x8[c][i],OLED_DATA);
		
	}
}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)
		result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}
			else 
				enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j],Char_Size);
		x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
	{
		OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
		adder+=1;
    }	
	OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
	{	
		OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
		adder+=1;
    }					
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
	unsigned int j=0;
	unsigned char x,y;

	if(y1%8==0) 
		y=y1/8;      
	else 
		y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
		for(x=x0;x<x1;x++)
		{      
			OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
		}
	}
} 

//初始化SSD1306					    
void OLED_Init(void)
{ 	 
 	GPIO_InitTypeDef  GPIO_InitStructure;  
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	     // 引脚外部链接了上拉电阻，可以专门提供高电平	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;     
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	IIC_SCL = 1;
	IIC_SDA_O = 1;
	

	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	printf("OLED_INIT_Success\n");
}  

