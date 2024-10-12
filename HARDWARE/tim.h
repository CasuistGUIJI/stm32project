#ifndef __TIM_H__
#define __TIM_H__

#include "stm32f4xx.h"
#include "led.h"


extern void TIM_Config(void);

extern void TIM1_UP_TIM10_IRQHandler(void);
extern void TIM2_IRQHandler(void);
extern void TIM3_IRQHandler(void);
extern void TIM8_UP_TIM13_IRQHandler(void);
#endif
