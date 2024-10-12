#include "sr04.h"
#include "beep.h"


void SR04_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;//定义一个GPIO结构体变量
	/* GPIOF Peripheral clock enable :使能GPIO端口F的外设时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  
	
	/* Configure PF9 in output pushpull mode ：配置GPIO的F端口的9号引脚为推晚输出模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;            //指定设置GPIO端口的引脚：9号引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;      //指定设置GPIO端口的模式：GPIO输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //指定设置GPIO端口的输出模式：推晚模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //指定设置GPIO端口的速度：高数100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   //指定设置GPIO端口的上/下拉：不使用模式
	//按照GPIO结构体的初始化配置GPIO端口F
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	
	/* Configure PF9 in output pushpull mode ：配置GPIO的E端口的9号引脚为推晚输出模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;            //指定设置GPIO端口的引脚：9号引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      //指定设置GPIO端口的模式：GPIO输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //指定设置GPIO端口的输出模式：推晚模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //指定设置GPIO端口的速度：高数100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   //指定设置GPIO端口的上/下拉：不使用模式
	//按照GPIO结构体的初始化配置GPIO端口F
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
}

uint16_t get_distance(void)
{
	uint16_t t=0;
	TRIG = 1;
	delay_us(20);
	TRIG = 0;
	//等待回响信号
	while(ECHO == 0);
	//检测回响持续时间
	while(ECHO)
	{
		delay_us(9);
		if(ECHO)
			t++;
	}
	return (3*t/2);
}
uint16_t distance;
void Distance_alarm(void)
{
    // 获取一次距离值，避免重复调用 get_distance()
    int distance = get_distance();

    // 根据距离值进行判断
    if (distance < 30)
    {
        PFout(9) = 0;
        PFout(10) = 0;
        PEout(13) = 0;
        PEout(14) = 0;
		SetPWM13_Frequency(263);
    }
    else if (distance >= 30 && distance < 50)
    {
        PFout(9) = 1;
        PFout(10) = 0;
        PEout(13) = 0;
        PEout(14) = 0;
		SetPWM13_Frequency(578);
    }
    else if (distance >= 50 && distance < 80)
    {
        PFout(9) = 1;
        PFout(10) = 1;
        PEout(13) = 0;
        PEout(14) = 0;
		SetPWM13_Frequency(687);
    }
    else if (distance >= 80 && distance < 110)
    {
        PFout(9) = 1;
        PFout(10) = 1;
        PEout(13) = 1;
        PEout(14) = 0;
		SetPWM13_Frequency(987);
    }
    else
    {
        PFout(9) = 1;
        PFout(10) = 1;
        PEout(13) = 1;
        PEout(14) = 1;
		SetPWM13_Frequency(0);
    }

    // 打印距离信息
    printf("距离 : %d mm\n", distance);
    
}


