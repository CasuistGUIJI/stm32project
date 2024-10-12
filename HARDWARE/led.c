#include "led.h"

LED_INFO led_info = {0};


void LED_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;//定义一个GPIO结构体变量
	/* GPIOF Peripheral clock enable :使能GPIO端口F的外设时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  
	
	/* Configure PF9 in output pushpull mode ：配置GPIO的F端口的9号引脚为推晚输出模式*/
	GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN;            //指定设置GPIO端口的引脚：9号引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      //指定设置GPIO端口的模式：GPIO输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //指定设置GPIO端口的输出模式：推晚模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //指定设置GPIO端口的速度：高数100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   //指定设置GPIO端口的上/下拉：不使用模式
	//按照GPIO结构体的初始化配置GPIO端口F
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	/* GPIOE Peripheral clock enable :使能GPIO端口E的外设时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	/* Configure PF9 in output pushpull mode ：配置GPIO的E端口的9号引脚为推晚输出模式*/
	GPIO_InitStructure.GPIO_Pin = LED3_PIN | LED4_PIN;            //指定设置GPIO端口的引脚：9号引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      //指定设置GPIO端口的模式：GPIO输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //指定设置GPIO端口的输出模式：推晚模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //指定设置GPIO端口的速度：高数100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   //指定设置GPIO端口的上/下拉：不使用模式
	//按照GPIO结构体的初始化配置GPIO端口F
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	//初始化熄灭LCD为熄灭
//	GPIO_SetBits(GPIOF,LED1_PIN | LED2_PIN);
//	GPIO_SetBits(GPIOE,LED3_PIN | LED4_PIN);
	PFout(9) = 1;
	PFout(10) = 1;
	PEout(13) = 1;
	PEout(14) = 1;
	printf("LED_INIT_Success \r\n");
}


void LED_Warning(void)
{
	PEout(14) = 0;
	delay_ms(200);

	PEout(14) = 1;
	delay_ms(200);
}

void LED_blink(void)
{
	PFout(9) = 0;
	PFout(10) = 0;
	PEout(13) = 0;
	PEout(14) = 0;
	delay_ms(1000);
	PFout(9) = 1;
	PFout(10) = 1;
	PEout(13) = 1;
	PEout(14) = 1;
	delay_ms(1000);
}

//void LED_GO(void)
//{
//	PFout(9) = 0;
//	delay(1000,1);
//	PFout(9) = 1;
//	
//	PFout(10) = 0;
//	delay(1000,1);
//	PFout(10) = 1;
//	
//	PEout(13) = 0;
//	delay(1000,1);
//	PEout(13) = 1;
//	
//	PEout(14) = 0;
//	delay(1000,1);
//	PEout(14) = 1;
//	delay(1000,1);

//}

void led_warning_th(void)
{
	LED3 = 0;
	LED2 = 0;
	PFout(8) = 1;
	delay_ms(100);
	
	LED3 = 1;
	LED2 = 1;
	PFout(8) = 0;
	delay_ms(100);
	
	LED3 = 0;
	LED2 = 0;
	PFout(8) = 1;
	delay_ms(200);
	
	LED3 = 1;
	LED2 = 1;
	PFout(8) = 0;
	delay_ms(200);
}

void led_warning_t(void)
{
	LED2 = 0;
	PFout(8) = 1;
	delay_ms(200);
	
	LED2 = 1;
	PFout(8) = 0;
	delay_ms(200);
	
	LED2 = 0;
	PFout(8) = 1;
	delay_ms(300);
	
	LED2 = 1;
	PFout(8) = 0;
	delay_ms(300);
}

void led_warning_h(void)
{
	LED3 = 0;
	PFout(8) = 1;
	delay_ms(300);
	
	LED3 = 1;
	PFout(8) = 0;
	delay_ms(300);
	
	LED3 = 0;
	PFout(8) = 1;
	delay_ms(300);
	
	LED3 = 1;
	PFout(8) = 0;
	delay_ms(300);
}

void Led0_Set(_Bool status)
{
	GPIO_WriteBit(GPIOF, GPIO_Pin_9, status == LED_ON ? Bit_RESET : Bit_SET);
	
	led_info.Led0_Status = status;
}

void Led1_Set(_Bool status)
{
	GPIO_WriteBit(GPIOF, GPIO_Pin_10, status == LED_ON ? Bit_RESET : Bit_SET);
	
	led_info.Led1_Status = status;
}

void Led2_Set(_Bool status)
{
	GPIO_WriteBit(GPIOE, GPIO_Pin_13, status == LED_ON ? Bit_RESET : Bit_SET);
	
	led_info.Led2_Status = status;
}

void Led3_Set(_Bool status)
{
	GPIO_WriteBit(GPIOE, GPIO_Pin_14, status == LED_ON ? Bit_RESET : Bit_SET);
	
	led_info.Led3_Status = status;
}