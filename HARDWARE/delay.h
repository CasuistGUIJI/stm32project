#ifndef __DELAY_H__
#define __DELAY_H__

#include "stm32f4xx.h"

extern void systick_init(uint8_t div);
extern void delay_s(uint32_t ns);
extern void delay_ms(uint32_t nms);
extern void delay_us(uint32_t nus);


#endif
