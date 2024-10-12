#include "iwdg.h"


void IWDG_config(uint16_t value)
{
	/* Enable the LSI oscillator ************************************************/
	RCC_LSICmd(ENABLE);
	
	/* Wait till LSI is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{
	}
	
	/* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
	dispersion) */
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	/* IWDG counter clock: LSI/32 :32000/32*/
	IWDG_SetPrescaler(IWDG_Prescaler_32);

	/* Set counter reload value to obtain 250ms IWDG TimeOut.
	IWDG counter clock Frequency = LsiFreq/32
	Counter Reload Value = 250ms/IWDG counter clock period
					= 0.25s / (32/LsiFreq)
					= LsiFreq/(32 * 4)
					= LsiFreq/128
	当前看门狗时钟频率1K hz
	*/   
	IWDG_SetReload(1000-1);  //大于设定的值则会复位

	/* Reload IWDG counter */
	IWDG_ReloadCounter();

	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);
	
	NVIC_InitTypeDef  NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);

	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 42000 -1;
	TIM_TimeBaseInitStruct.TIM_Period = value*2-1;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM7,ENABLE);
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
	
}



void TIM7_config(uint16_t value)
{
	NVIC_InitTypeDef  NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);

	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 42000 -1;
	TIM_TimeBaseInitStruct.TIM_Period = value*2-1;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM7,ENABLE);
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
}

/* TIM7 interrupt handler */
void TIM7_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
    {
        /* Refresh IWDG counter to prevent reset */
        IWDG_ReloadCounter();
		/* Clear interrupt flag */
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    }
}


void WWDG_config(void)
{
	 /* Enable WWDG clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	NVIC_InitTypeDef  NVIC_InitStruct;
	/* WWDG clock counter = (PCLK1 (42MHz)/4096)/8 = 1281 Hz (~780 us)  */
	WWDG_SetPrescaler(WWDG_Prescaler_8);

	/* Set Window value to 80; WWDG counter should be refreshed only when the counter
	 is below 80 (and greater than 64) otherwise a reset will be generated */
	WWDG_SetWindowValue(80);

	/* Enable WWDG and set counter value to 127, WWDG timeout = ~780 us * 64 = 49.92 ms 
	  In this case the refresh window is: 
			~780 * (127-80) = 36.6ms < refresh window < ~780 * 64 = 49.9ms
	*/
	WWDG_Enable(127);
	
	NVIC_InitStruct.NVIC_IRQChannel = WWDG_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	// Clear the interrupt flag if it was set
	WWDG_ClearFlag(); 
	 /* Enable WWDG interrupt */
    WWDG_EnableIT();
}

void WWDG_IRQHandler(void)
{
	if(WWDG_GetFlagStatus() != RESET)
	{
		 // 打印调试信息到串口
        printf("WWDG triggered!\r\n");
//		delay_ms(40);
		/* Update WWDG counter */
		WWDG_SetCounter(127);
	}
	WWDG_ClearFlag();
}
