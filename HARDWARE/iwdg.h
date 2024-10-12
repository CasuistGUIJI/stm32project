#ifndef __IWDG_H___
#define __IWDG_H___

#include "stm32f4xx.h"
//tim7合并版
extern void IWDG_config(uint16_t value);
//可不要
extern void TIM7_config(uint16_t value);

extern void WWDG_config(void);



#endif