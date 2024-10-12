/*
 * @Author: CasuistGUIJI 2483690428@qq.com
 * @Date: 2024-09-06 17:32:35
 * @LastEditors: CasuistGUIJI 2483690428@qq.com
 * @LastEditTime: 2024-09-10 10:19:12
 * @FilePath: \undefinede:\YQshare\08_stm32\08_UART\HARDWARE\pwm.c
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include "pwm.h"

int v1 = 0;
int v2 = 100;
int32_t  step = 1;
int32_t  step2 = -1;

uint16_t  value = 0;
//volatile uint8_t isPaused;

//led1呼吸灯
void TIM14_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
    /* 使能 TIM14 和 GPIOF 时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	/* 配置 GPIOF 的 9 号引脚为复用推挽模式，输出PWM */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  // LED引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; // 复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 高速
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 无上下拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* 连接 TIM14 的通道到 PF9 */
   	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);

    /* TIM14 配置: 100Hz PWM频率 (10ms的周期)，控制LED亮度 */
    TIM_TimeBaseStructure.TIM_Period = 10000/100 - 1;  // 100Hz 频率
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1;    // 预分频器，将时钟降到 10kHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

	/* PWM1 模式配置：通道1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  // PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;  // 初始占空比为0
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 高电平有效

	TIM_OC1Init(TIM14, &TIM_OCInitStructure);
   	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable); // 使能通道1预装载

	/* 使能 TIM14 自动重载 */
	TIM_ARRPreloadConfig(TIM14, ENABLE);

    /* 清除中断标志并使能 TIM14 */
	TIM_ClearFlag(TIM14, TIM_FLAG_Update);
	TIM_Cmd(TIM14, ENABLE); // 开始计数
}

/* 改变PWM占空比的函数，用于控制LED的亮暗变化 */
void SetPWM_DutyCycle14(uint16_t duty)
{
    TIM_SetCompare1(TIM14, duty);  // 设置TIM14通道1的占空比
}


void TIM1_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
    /* 使能 TIM1 和 GPIOE 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	/* 配置 GPIOF 的 9 号引脚为复用推挽模式，输出PWM */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;  // LED引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; // 复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 高速
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 无上下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* 连接 TIM1 的通道到 PE13 */
   	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);

    /* TIM14 配置: 100Hz PWM频率 (10ms的周期)，控制LED亮度 */
    TIM_TimeBaseStructure.TIM_Period = 10000/100 - 1;  // 100Hz 频率
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1;    // 预分频器，将时钟降到 10kHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	/* PWM1 模式配置：通道1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  // PWM模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;  // 初始占空比为0
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 高电平有效

	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
   	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable); // 使能通道1预装载
	
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
   	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable); // 使能通道1预装载

	/* 使能 TIM14 自动重载 */
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	
	TIM_Cmd(TIM1, ENABLE); // 开始计数
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}

void SetTIM1_LED3_LED4(uint16_t brightness_ch3, uint16_t brightness_ch4)
{
    TIM_SetCompare3(TIM1, brightness_ch3);  // 设置CH3的占空比
    TIM_SetCompare4(TIM1, brightness_ch4);  // 设置CH4的占空比
}


void TIM13_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
    /* 使能 TIM13 和 GPIOF 时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	/* 配置 GPIOF 的 8 号引脚为复用推挽模式，输出PWM */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  // LED引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; // 复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 高速
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 无上下拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* 连接 TIM13 的通道到 PF8 */
   	GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_TIM13);

    /* TIM13 配置: 100Hz PWM频率 (10ms的周期)，控制LED亮度 */
    TIM_TimeBaseStructure.TIM_Period = 10000/10 - 1;  // 100Hz 频率
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1;    // 预分频器，将时钟降到 10kHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);

	/* PWM1 模式配置：通道1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  // PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;  // 初始占空比为0
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 高电平有效

	TIM_OC1Init(TIM13, &TIM_OCInitStructure);
   	TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable); // 使能通道1预装载

	
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure NVIC for USART1 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	/* 使能 TIM14 自动重载 */
	TIM_ARRPreloadConfig(TIM13, ENABLE);

    /* 清除中断标志并使能 TIM14 */
	TIM_ClearFlag(TIM13, TIM_FLAG_Update);
	TIM_Cmd(TIM13, ENABLE); // 开始计数
}

//void TIM8_UP_TIM13_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM13,TIM_IT_Update) != RESET)
//	{
//		delay_ms(20);
//		if(PAin(0) == 0)
//		{
//			PFout(8) ^=1;
//		}
//		TIM_ClearITPendingBit(TIM13,TIM_IT_Update);
//	}
//}


void SetPWM13_Frequency(uint16_t frequency)
{
    if (frequency == 0)
    {
        TIM_SetAutoreload(TIM13, 0xFFFF);  // 设置周期为最大值
        TIM_SetCompare1(TIM13, 0);  // 设置占空比为0
    }
    else
    {
        uint32_t period = 10000 / frequency - 1;  // 计算周期
        TIM_SetAutoreload(TIM13, period);  // 设置定时器周期
        TIM_SetCompare1(TIM13, period / 2);  // 占空比为50%
    }
}

void Breathing_light(void)
{
	v1 += step;
	
	if (v1 == 0 || v1 ==100)
	{
		step = -step; // 反转方向
	}
	
	v2 += step2;
	if (v2 == 100 || v2 ==0)
	{
		step2 = -step2; // 反转方向
	}
	delay_ms(12);
	SetTIM1_LED3_LED4(v1,v2);
//		
	
//		SetPWM_DutyCycle14(value);
//		// 控制亮度变化
//		value += step;
//		if (value >= 100 || value <=0)
//		{
//			step = -step; // 反转方向
//		}
//		// 延时控制呼吸速度
//		delay_ms(12);  // 调整延时以控制呼吸速度
}

void music(void)
{
	int music[7] = {261, 293, 329, 349, 392, 440, 494};
	//25
	int happyBirthdayMelody[] = {
    392, 392, 440, 392, 523, 494,   // 祝你生日快乐，第一句
    392, 392, 440, 392, 587, 523,   // 祝你生日快乐，第二句
    392, 392, 784, 659, 523, 494, 440,   // 祝你生日快乐，第三句
    698, 698, 659, 523, 587, 523    // 祝你生日快乐，最后一句
	};
	//29
	int qingtianMelody[] = {
    330, 392, 440, 392, 330, 392, 440, 392,   // 3 5 6   5 3 5 6
    330, 392, 440, 392, 330, 392, 440, 392,   // 5 3 5 6   5 3 5 6 5
    330, 392, 440, 494, 440, 392, 330, 392,   // 3 5 6   7 6 5 3 5
    440, 392, 330, 392, 440                   // 6 5 3 5 6
	};
	//26
	int shouxiedecongqianMelody[] = {
    440, 523, 587, 659, 587, 523, 440,   // 6 1' 2' 3' 2' 1' 6
    523, 587, 659, 587, 523, 440,        // 1' 2' 3' 2' 1' 6
    659, 698, 659, 587, 523, 440,        // 3' 4' 3' 2' 1' 6
    392, 440, 523, 587, 659, 587, 523    // 5 6 1' 2' 3' 2' 1'
	};
	//42
	int twinkleTwinkleMelody[] = {
    261, 261, 392, 392, 440, 440, 392,  // 1 1 5 5 6 6 5
    349, 349, 329, 329, 293, 293, 261,  // 4 4 3 3 2 2 1
    392, 392, 349, 349, 329, 329, 293,  // 5 5 4 4 3 3 2
    392, 392, 349, 349, 329, 329, 293,  // 5 5 4 4 3 3 2
    261, 261, 392, 392, 440, 440, 392,  // 1 1 5 5 6 6 5
    349, 349, 329, 329, 293, 293, 261   // 4 4 3 3 2 2 1
	};
			
	int qiliXiang[] = {
		261, 329, 392, 392, 440, 392, 329,
	   261, 329, 392, 392, 440, 392, 329,
	   440, 493, 523, 523, 493, 440, 392,
	   349, 392, 440, 392, 349, 329, 293
	};
	for (int i = 0; i < sizeof(qingtianMelody) / sizeof(qingtianMelody[0]); i++)
	{
		SetPWM13_Frequency(qingtianMelody[i]);  // 播放当前音符
		delay_ms(400);  // 控制每个音符的持续时间
		
	}
}
