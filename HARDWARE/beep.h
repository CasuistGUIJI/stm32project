#ifndef __BEEP_H__
#define __BEEP_H__

#include "stm32f4xx.h"
#include "bit_band.h"
#include "delay.h"

#define BEEP   GPIO_Pin_8
extern void BEEP_init(void);
extern void beep_warning(void);

#endif
