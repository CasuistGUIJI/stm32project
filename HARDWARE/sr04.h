#ifndef __SR04_H__
#define __SR04_H__

#include "stm32f4xx.h"
#include "delay.h"
#include "bit_band.h"
#include "led.h"
#include "pwm.h"
#include "usart.h"


#define TRIG PDout(7)
#define ECHO PDin(6)

extern void SR04_init(void);
extern uint16_t get_distance(void);
extern void Distance_alarm(void);

#endif
