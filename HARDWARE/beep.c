#include "beep.h"

void BEEP_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;//定义一个GPIO结构体变量
	/* GPIOF Peripheral clock enable :使能GPIO端口F的外设时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  
	
	/* Configure PF9 in output pushpull mode ：配置GPIO的F端口的9号引脚为推晚输出模式*/
	GPIO_InitStructure.GPIO_Pin = BEEP;            //指定设置GPIO端口的引脚：9号引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      //指定设置GPIO端口的模式：GPIO输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //指定设置GPIO端口的输出模式：推晚模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //指定设置GPIO端口的速度：高数100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   //指定设置GPIO端口的上/下拉：不使用模式
	//按照GPIO结构体的初始化配置GPIO端口F
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	PFout(8) = 0;
	printf("BEEP_INIT_Success \r\n");
}


void beep_warning(void)
{
	for(int i =0;i<5;i++)
	{
		PFout(8) = 0;
		delay_ms(300);
		PFout(8) = 1;
		delay_ms(300);
	}
	GPIO_ResetBits(GPIOF, GPIO_Pin_8);
	delay_ms(200);
	GPIO_SetBits(GPIOF, GPIO_Pin_8);
	delay_ms(200);
	GPIO_ResetBits(GPIOF, GPIO_Pin_8);
	delay_ms(200);
	GPIO_SetBits(GPIOF, GPIO_Pin_8);
	delay_ms(200);
	GPIO_ResetBits(GPIOF, GPIO_Pin_8);
	delay_ms(200);
}