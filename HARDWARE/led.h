#ifndef __LED_H__
#define __LED_H__


#include "stm32f4xx.h"
#include "bit_band.h"
#include "delay.h"

typedef struct
{
	_Bool Led0_Status;
	_Bool Led1_Status;
	_Bool Led2_Status;
	_Bool Led3_Status;

} LED_INFO;

#define LED_ON	1

#define LED_OFF	0

extern LED_INFO led_info;


#define LED1_PIN   GPIO_Pin_9 
#define LED2_PIN   GPIO_Pin_10
#define LED3_PIN   GPIO_Pin_13 
#define LED4_PIN   GPIO_Pin_14

#define LED0 PFout(9) 
#define LED1 PFout(10)	
#define LED2 PEout(13) 
#define LED3 PEout(14)	

extern void LED_init(void);
extern void LED_blink(void);
extern void LED_GO(void);
extern void LED_Warning(void);

extern void led_warning_th(void);

extern void led_warning_t(void);
extern void led_warning_h(void);

extern void Led0_Set(_Bool status);

extern void Led1_Set(_Bool status);

extern void Led2_Set(_Bool status);
extern void Led3_Set(_Bool status);

#endif
