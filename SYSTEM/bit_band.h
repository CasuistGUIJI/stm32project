/*** 
 * @Author: CasuistGUIJI 2483690428@qq.com
 * @Date: 2024-09-04 14:23:28
 * @LastEditors: CasuistGUIJI 2483690428@qq.com
 * @LastEditTime: 2024-09-06 16:28:41
 * @FilePath: \USERe:\YQshare\08_stm32\05_Exceptions_interrupts\SYSTEM\bit_band.h
 * @Description: 
 * @
 * @Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef __BIT_BAND_H__
#define __BIT_BAND_H__

#include "led.h"


//位带操作
//IO口操作宏定义
#define BITBAND(addr, bitnum)  	((addr & 0xF0000000)+0x2000000+((addr & 0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  		*((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)	MEM_ADDR(BITBAND(addr, bitnum)) 

//输出数据寄存器IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

//输入数据寄存器IO口地址映射
#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 

//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //输出 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //输入

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入


//#define PAout(x)  *((volatile uint32_t *)(0x42000000 + ((uint32_t)(&GPIOA->ODR) - 0x40000000)*32 + x*4))
//#define PAin(x)  *((volatile uint32_t *)(0x42000000 + ((uint32_t)(&GPIOA->IDR) - 0x40000000)*32 + x*4))

//#define PBout(x)  *((volatile uint32_t *)(0x42000000 + ((uint32_t)(&GPIOB->ODR) - 0x40000000)*32 + x*4))
//#define PBin(x)  *((volatile uint32_t *)(0x42000000 + ((uint32_t)(&GPIOB->IDR) - 0x40000000)*32 + x*4))

//#define PCout(x)  *((volatile uint32_t *)(0x42000000 + ((uint32_t)(&GPIOC->ODR) - 0x40000000)*32 + x*4))
//#define PCin(x)  *((volatile uint32_t *)(0x42000000 + ((uint32_t)(&GPIOC->IDR) - 0x40000000)*32 + x*4))

//#define PDout(x)  *((volatile uint32_t *)(0x42000000 + ((uint32_t)(&GPIOD->ODR) - 0x40000000)*32 + x*4))
//#define PDin(x)  *((volatile uint32_t *)(0x42000000 + ((uint32_t)(&GPIOD->IDR) - 0x40000000)*32 + x*4))

//#define PEout(x)  *((volatile uint32_t *)(0x42000000 + ((uint32_t)(&GPIOE->ODR) - 0x40000000)*32 + x*4))
//#define PEin(x)  *((volatile uint32_t *)(0x42000000 + ((uint32_t)(&GPIOE->IDR) - 0x40000000)*32 + x*4))

//#define PFout(x)  *((volatile uint32_t *)(0x42000000 + ((uint32_t)(&GPIOF->ODR) - 0x40000000)*32 + x*4))
//#define PFin(x)  *((volatile uint32_t *)(0x42000000 + ((uint32_t)(&GPIOF->IDR) - 0x40000000)*32 + x*4))

//#define PGout(x)  *((volatile uint32_t *)(0x42000000 + ((uint32_t)(&GPIOG->ODR) - 0x40000000)*32 + x*4))
//#define PGin(x)  *((volatile uint32_t *)(0x42000000 + ((uint32_t)(&GPIOG->IDR) - 0x40000000)*32 + x*4))

//#define PHout(x)  *((volatile uint32_t *)(0x42000000 + ((uint32_t)(&GPIOH->ODR) - 0x40000000)*32 + x*4))
//#define PHin(x)  *((volatile uint32_t *)(0x42000000 + ((uint32_t)(&GPIOH->IDR) - 0x40000000)*32 + x*4))

#endif
