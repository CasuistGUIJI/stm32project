/*
 * @Author: CasuistGUIJI 2483690428@qq.com
 * @Date: 2024-09-06 17:32:35
 * @LastEditors: CasuistGUIJI 2483690428@qq.com
 * @LastEditTime: 2024-09-07 11:28:41
 * @FilePath: \USERe:\YQshare\08_stm32\06_interrupts\HARDWARE\tim.c
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include "tim.h"

void TIM_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    /* 使能 TIM2 和 TIM3 的时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_TIM8, ENABLE);
	
	
    /* TIM1 配置: 100ms 定时中断 */
    TIM_TimeBaseStructure.TIM_Period = 10000/10 - 1;            // 1000个计数对应100ms(10Hz即100ms触发一次)
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1;             // 预分频器：8400(在换算时会自动加1)；时钟降到 10kHz（84MHz/8400）
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    /* TIM2 配置: 200ms 定时中断 */
    TIM_TimeBaseStructure.TIM_Period = 2000 - 1;            // 2000个计数对应200ms
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	/* TIM3 配置: 500ms 定时中断 */
    TIM_TimeBaseStructure.TIM_Period = 5000 - 1;            // 5000个计数对应500ms
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/* TIM8 配置: 2000ms 定时中断 */
    TIM_TimeBaseStructure.TIM_Period = 20000 - 1;            // 20000个计数对应2000ms
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

    /* 配置中断优先级 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    /* 清除中断标志 */
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_ClearFlag(TIM8, TIM_FLAG_Update);

    /* 使能 TIM2 和 TIM3 的更新中断 */
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);

    /* 使能 TIM2 和 TIM3 的计数器 */
	TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM8, ENABLE);
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	// static u8 t =0 ;
	if(TIM_GetITStatus(TIM1,TIM_IT_Update) != RESET)
	{
		// t++;
		// if(t == 4)
		// {
		// 	t =0;
			PFout(9) ^= 1;
			TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
		// }
	}
}

void TIM2_IRQHandler(void)
{
	// static u8 t =0 ;
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		// t++;
		// if(t == 4)
		// {
			// t =0;
			PFout(10) ^= 1;
			TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		// }
	}
}
void TIM3_IRQHandler(void)
{
	// static u8 t =0 ;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
	{
		
		//////////
		PEout(13) ^= 1;
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

	}
}

void TIM8_UP_TIM13_IRQHandler(void)
{
	// static u8 t =0 ;
	if(TIM_GetITStatus(TIM8,TIM_IT_Update) != RESET)
	{
		// t++;
		// if(t == 4)
		// {
			// t =0;
			PEout(14) ^= 1;
			TIM_ClearITPendingBit(TIM8,TIM_IT_Update);
		// }
	}
}
