#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f4xx.h"
#include "usart.h"
#include "led.h"

extern void ADC_Config(void);
extern void ADC_photosensitive_Config(void);
extern uint16_t ADC_convert(ADC_TypeDef* ADCx,uint32_t ADC_Resolution);



#endif