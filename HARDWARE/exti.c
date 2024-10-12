#include "exti.h"
volatile uint8_t key_value=0;

void EXTI_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;  // 定义 EXTI 初始化结构体
	NVIC_InitTypeDef   NVIC_InitStructure;  // 定义 NVIC 初始化结构体

	/* Enable SYSCFG clock ：使能 SYSCFG 时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);  // 使能 SYSCFG 外设时钟

	/* Connect EXTI Line0 to PA0 pin ：将 EXTI 线路 0 连接到 PA0 引脚 */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);  // 配置 EXTI 线路 0 对应 GPIOA 的 0 号引脚
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);	// 配置 EXTI 线路 2 对应 GPIOE 的 2 号引脚
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);	// 配置 EXTI 线路 3 对应 GPIOE 的 3 号引脚
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);	// 配置 EXTI 线路 4 对应 GPIOE 的 4 号引脚
	/* Configure EXTI Line0 ：配置 EXTI 线路 0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line2 | EXTI_Line3 | EXTI_Line4;   // 选择 EXTI 线路 0,2,3,4
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;          // 设置为中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       // 设置为上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                    // 使能 EXTI 线路
	EXTI_Init(&EXTI_InitStructure);                              // 初始化 EXTI 线路

	/* Enable and set EXTI Line0 Interrupt to the lowest priority ：
	   使能并设置 EXTI 线路 0 的中断，优先级设为最低 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;             // 选择 EXTI0 中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; // 设置抢占优先级为 0（最高）
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;        // 设置子优先级为 0（最高）
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              // 使能 NVIC 中断通道
	NVIC_Init(&NVIC_InitStructure);                              // 初始化 NVIC

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // 设置抢占优先级为 0（最高）
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // 设置子优先级为 0（最高）
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}

//中断请求响应函数：该函数没有参数没有返回值且不能手动调用，函数名字不能随便写

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		delay_ms(50);
		if(PAin(0) == 0)
		{
			key_value = 1;  // 切换暂停状态
			PFout(9) ^=1;
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

/*中断请求响应函数：该函数没有参数没有返回值且不能手动调用，
函数名字不能随便写且函数功能必须简洁高效*/
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		delay_ms(50);
		if(PEin(2) == 0)
		{
			key_value = 2;
			PFout(10) ^=1;
		}
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		delay_ms(50);
		if(PEin(3) == 0)
		{
			key_value = 3;
			PEout(13) ^=1;
		}
		
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		delay_ms(50);
		if(PEin(4) == 0)
		{
			key_value = 4;
			PEout(14) ^=1;
		}
		
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}
