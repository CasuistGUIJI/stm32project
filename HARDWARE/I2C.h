#ifndef __I2C_H__
#define __I2C_H__

#include "stm32f4xx.h"
#include <string.h>
#include <stdlib.h>
#include "usart.h"
#include "led.h"
#include "delay.h"
#include "bit_band.h"

#define  IIC_SCL   	 PBout(8)
#define  IIC_SDA_O   PBout(9)
#define  IIC_SDA_I   PBin(9)

// I2C 初始化函数
extern void i2c_Init(void);
// 配置 SDA 引脚的工作模式
extern void SDA_Mode(GPIOMode_TypeDef mode);
// 起始信号
extern void i2c_Start(void);
// 停止信号
extern void i2c_Stop(void);
// 发送数据
extern void i2c_SendByte(uint8_t data);
// 接收数据
extern uint8_t i2c_RecvByte(void);
// 接收ACK信号
extern uint8_t i2c_Recv_ack(void);
// 发送ACK信号
extern void i2c_Send_ack(uint8_t ack);
// AT24C02写入数据
extern void AT24C02_Write(uint8_t addr, uint8_t *pbuf, uint8_t len);
// AT24C02读取数据
extern void AT24C02_Read(uint8_t addr, uint8_t *pbuf, uint8_t len);

#endif
