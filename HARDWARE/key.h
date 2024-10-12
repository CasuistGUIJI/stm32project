/*** 
 * @Author: CasuistGUIJI 2483690428@qq.com
 * @Date: 2024-09-03 18:32:09
 * @LastEditors: CasuistGUIJI 2483690428@qq.com
 * @LastEditTime: 2024-09-03 18:39:39
 * @FilePath: \USERe:\YQshare\08_stm32\02_template\HARDWARE\key.h
 * @Description: 
 * @
 * @Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f4xx.h"
#include "bit_band.h"
#include "delay.h"

#define   KEY1_PIN   GPIO_Pin_0
#define   KEY2_PIN   GPIO_Pin_2
#define   KEY3_PIN   GPIO_Pin_3
#define   KEY4_PIN   GPIO_Pin_4

extern volatile uint8_t key_value;

extern void KEY_init(void);
extern uint8_t key_scan(void);
//extern void KEY_Exti_init(void);

#endif 
