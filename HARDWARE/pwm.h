#ifndef __PWM_H__
#define __PWM_H__

#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "delay.h"

//TIM1的通道3和通道4对应控制led3和led4
extern void TIM1_Config(void);
extern void SetTIM1_LED3_LED4(uint16_t brightness_ch3, uint16_t brightness_ch4);

//蜂鸣器
extern void TIM13_Config(void);
extern void SetPWM_DutyCycle13(uint16_t duty);
//更新设置的频率变换成对应的周期
extern void SetPWM13_Frequency(uint16_t frequency);

//呼吸灯
extern void TIM14_Config(void);
extern void SetPWM_DutyCycle14(uint16_t duty);

extern void music(void);
extern void Breathing_light(void);


//extern void TIM8_UP_TIM13_IRQHandler(void);
#endif
