/*
 * @Author: CasuistGUIJI 2483690428@qq.com
 * @Date: 2024-09-23 15:40:39
 * @LastEditors: CasuistGUIJI 2483690428@qq.com
 * @LastEditTime: 2024-09-23 15:42:21
 * @FilePath: \19_IIC\HARDWARE\I2C.c
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include "I2C.h"


// I2C 初始化函数
void i2c_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; // 定义 GPIO 初始化结构体
    // 使能 GPIOB 时钟，准备使用 GPIOB 引脚
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
    // 配置 GPIO 引脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;  // SCL 和 SDA 引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;            // 设置为输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;          // 设置为开漏输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;      // 设置输出速率为 100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        // 无上下拉电阻
    GPIO_Init(GPIOB, &GPIO_InitStructure);                   // 初始化 GPIOB
    
    IIC_SCL = 1; // 将 SCL 引脚拉高，保持空闲状态
    IIC_SDA_O = 1; // 将 SDA 引脚拉高，保持空闲状态
}

// 配置 SDA 引脚的工作模式
void SDA_Mode(GPIOMode_TypeDef mode)
{
    GPIO_InitTypeDef  GPIO_InitStructure; // 定义 GPIO 初始化结构体

    // 配置 SDA 引脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  // 指定 SDA 引脚
    GPIO_InitStructure.GPIO_Mode = mode;        // 设置为输入或输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; // 设置为开漏输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 设置输出速率为 100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 无上下拉电阻
    GPIO_Init(GPIOB, &GPIO_InitStructure);       // 初始化 GPIOB
}


// 起始信号
void i2c_Start(void)
{
    SDA_Mode(GPIO_Mode_OUT); // 设置 SDA 为输出模式
    
    IIC_SCL = 1; // 将 SCL 拉高
    IIC_SDA_O = 1; // 将 SDA 拉高
    delay_us(5); // 确保稳定
    
    IIC_SDA_O = 0; // 将 SDA 拉低，产生起始信号
    delay_us(5); // 确保稳定
}


// 停止信号
void i2c_Stop(void)
{
    SDA_Mode(GPIO_Mode_OUT); // 设置 SDA 为输出模式
    
    IIC_SCL = 1; // 将 SCL 拉高
    IIC_SDA_O = 0; // 将 SDA 拉低
    delay_us(5); // 确保稳定
    
    IIC_SDA_O = 1; // 将 SDA 拉高，产生停止信号
    delay_us(5); // 确保稳定
}


// 发送数据
void i2c_SendByte(uint8_t data)
{
    SDA_Mode(GPIO_Mode_OUT); // 将 SDA 设置为输出模式
    IIC_SCL = 0; // 将 SCL 拉低，准备发送数据

    for(u8 i=0; i<8; i++)
    {
        if(data & (1<<(7-i)))
            IIC_SDA_O = 1; // 根据数据位设置 SDA
        else
            IIC_SDA_O = 0; // 设置 SDA 低
        
        delay_us(5); // 确保稳定
        
        IIC_SCL = 1; // 将 SCL 拉高，发送数据位
        delay_us(5); // 确保稳定
        
        IIC_SCL = 0; // 将 SCL 拉低，准备发送下一个数据位
        delay_us(5); // 确保稳定
    }
}


// 接收数据
uint8_t i2c_RecvByte(void)
{
    uint8_t data = 0;
    
    SDA_Mode(GPIO_Mode_IN); // 将 SDA 设置为输入模式

    for(u8 i=0; i<8; i++)
    {
        IIC_SCL = 1; // SCL 拉高，准备读取数据
        delay_us(5);
        if(IIC_SDA_I) // 读取 SDA 的状态
            data |= (1<<(7-i)); // 将读取的位保存到 data
        
        IIC_SCL = 0; // SCL 拉低
        delay_us(5); // 确保稳定
    }
    
    return data; // 返回读取的数据
}


// 接收ACK信号
uint8_t i2c_Recv_ack(void)
{
    uint8_t ack;

    // 将SDA配置为输入模式，以接收ACK
    SDA_Mode(GPIO_Mode_IN);

    IIC_SCL = 1; // 拉高SCL，准备读取ACK
    delay_us(5); // 确保稳定

    // 检查SDA的状态，确定ACK
    ack = IIC_SDA_I ? 1 : 0;

    // 将SCL拉低，结束ACK接收
    IIC_SCL = 0;

    return ack; // 返回ACK状态
}

// 发送ACK信号
void i2c_Send_ack(uint8_t ack)
{
    // 将SDA配置为输出模式
    SDA_Mode(GPIO_Mode_OUT);

    IIC_SCL = 0; // 拉低SCL，准备发送ACK

    // 根据ack的值设置SDA
    IIC_SDA_O = ack ? 1 : 0;
    
    IIC_SCL = 1; // 拉高SCL，发送ACK
    delay_us(5); // 确保稳定

    IIC_SCL = 0; // 拉低SCL，准备下一次操作
}

// AT24C02写入数据
void AT24C02_Write(uint8_t addr, uint8_t *pbuf, uint8_t len)
{
    // 发送起始信号
    i2c_Start();

    // 发送设备地址（写模式）
    i2c_SendByte(0xA0);

    // 接收从机应答ACK
    if (i2c_Recv_ack())
    {
        printf("寻设备地址失败!\r\n");
        return;
    }

    // 发送写数据的地址
    i2c_SendByte(addr);

    // 接收从机应答ACK
    if (i2c_Recv_ack())
    {
        printf("访问设备地址失败!\r\n");
        return;
    }

    // 循环发送数据
    while (len--)
    {
        // 发送数据
        i2c_SendByte(*pbuf);

        // 接收从机应答ACK
        if (i2c_Recv_ack())
        {
            printf("数据存储失败!\r\n");
            return;
        }

        pbuf++; // 指向下一个字节
    }

    // 发送停止信号
    i2c_Stop();
    delay_ms(5); // 等待写入完成
}

// AT24C02读取数据
void AT24C02_Read(uint8_t addr, uint8_t *pbuf, uint8_t len)
{
    // 发送起始信号
    i2c_Start();

    // 发送设备地址（写模式）
    i2c_SendByte(0xA0);

    // 接收从机应答ACK
    if (i2c_Recv_ack())
    {
        printf("寻设备地址失败!\r\n");
        return;
    }

    // 发送要读取的地址
    i2c_SendByte(addr);

    // 接收从机应答ACK
    if (i2c_Recv_ack())
    {
        printf("访问设备地址失败!\r\n");
        return;
    }

    // 发送起始信号，准备读取
    i2c_Start();

    // 发送设备地址（读模式）
    i2c_SendByte(0xA1);

    // 接收从机应答ACK
    if (i2c_Recv_ack())
    {
        printf("寻设备地址失败!\r\n");
        return;
    }

    len--; // 读取的最后一个字节将会单独处理
    while (len--)
    {
        // 接收数据
        *pbuf = i2c_RecvByte();

        // 发送应答ACK，告知继续读取
        i2c_Send_ack(0);

        pbuf++; // 指向下一个字节
    }

    // 接收最后一个字节
    *pbuf = i2c_RecvByte();

    // 发送无应答ACK，结束读取
    i2c_Send_ack(1);

    // 发送停止信号
    i2c_Stop();
}
