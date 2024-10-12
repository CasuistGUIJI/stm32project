#ifndef __EXTI_H__
#define __EXTI_H__

#include "stm32f4xx.h"
#include "led.h"
#include "pwm.h"
#include "key.h"

extern void EXTI_Config(void);
extern void EXTI0_IRQHandler(void);
extern void EXTI2_IRQHandler(void);
extern void EXTI3_IRQHandler(void);
extern void EXTI4_IRQHandler(void);

#endif
