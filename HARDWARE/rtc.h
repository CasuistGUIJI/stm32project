#ifndef __RTC_H__
#define __RTC_H__

#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "beep.h"



extern volatile u8 RTC_Wakeup_event_flag;

extern void RTC_INITConfig(void);

extern void RTC_Alarm_Config(void);
extern void RTC_TimeShow(void);
extern void RTC_WakeUp_Config(void);
extern void wakeup_show(void);


#endif
