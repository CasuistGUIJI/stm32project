#include "key.h"

//volatile uint8_t key_value=0;

void KEY_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* GPIOA Peripheral clock enable：使能GPIO端口A的外围设备时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
	
	/* Configure PA0 in intput mode：配置GPIO的A端口的0号引脚为推输入模式 */
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN;  // 指定设置GPIO端口的引脚：0号引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		// 指定设置GPIO端口的模式：GPIO输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    // 指定设置GPIO端口的上/下拉模式：不使用，浮空
	GPIO_Init(GPIOA, &GPIO_InitStructure); // 按照GPIO结构体的初始化配置GPIO端口F
	 
	GPIO_InitStructure.GPIO_Pin =  KEY2_PIN | KEY3_PIN | KEY4_PIN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	printf("KEY_INIT_Success \r\n");
}

uint8_t key_scan(void)
{
	// 高电平为1 低电平为0
	if(PAin(0) == 0)  // 按键按下
		return 1;
	if(PEin(2) == 0)  // 按键按下
		return 2;
	if(PEin(3) == 0)  // 按键按下
		return 3;
	if(PEin(4) == 0)  // 按键按下
		return 4;
	
	return 0;
}

//void KEY_Exti_init(void)
//{
//	EXTI_InitTypeDef   EXTI_InitStructure;
//	NVIC_InitTypeDef   NVIC_InitStructure;

//	
//	/* Enable SYSCFG clock：使能SYSCFG时钟 */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//	
//	KEY_init();
//	
//	/* Connect EXTI Line0 to PA0 pin：指定链接GPIO端口A到中断线0（PA0 --> EXTILine0）*/
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
//	
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);

//	/* Configure EXTI Line0：配置中断线 */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line2 | EXTI_Line3 | EXTI_Line4;				// 指定配置外部中断/事件控制线：线0\2\3\4
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		// 指定外部中断/事件控制线的模式：中断模式
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  // 指定线的触发边沿：上升沿
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				// 指定线的新状态：使能（开启）
//	EXTI_Init(&EXTI_InitStructure); // 按照结构体的初始化去配置硬件

//	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;             // 指定配置中断通道：外部事件/中断线0
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;    // 指定中断通道的抢占优先级：最低
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;           // 指定中断通道的响应优先级：最低
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 // 指定中断通道触发信号状态：使能
//	NVIC_Init(&NVIC_InitStructure); // 按照结构体的初始化去配置硬件
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;             // 指定配置中断通道：外部事件/中断线2
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;             // 指定配置中断通道：外部事件/中断线3
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;             // 指定配置中断通道：外部事件/中断线4
//	NVIC_Init(&NVIC_InitStructure);
//}

//// 中断请求响应函数：该函数没有参数没有返回值且不能手动调用，函数的名字不能随意写且函数功能尽量高效简介
//void EXTI0_IRQHandler(void)
//{
//	// 获取中断标志：进一步确认中断是否触发
//	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
//	{
//		delay_ms(20);  // 消除抖动，跳过按键的不稳定时间
//		if(PAin(0) == 0)
//			/* Toggle LED1 */
//			key_value = 1;
//		
//		/* Clear the EXTI line 0 pending bit：清除中断触发标志，允许下次触发 */
//		EXTI_ClearITPendingBit(EXTI_Line0);
//	}
//}

//void EXTI2_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
//	{
//		
//		delay_ms(20);
//		if(PEin(2) == 0)
//			/* Toggle LED1 */
//			key_value = 2;
//		
//		/* Clear the EXTI line 2 pending bit */
//		EXTI_ClearITPendingBit(EXTI_Line2);
//	}
//}

//void EXTI3_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
//	{
//		delay_ms(20);
//		if(PEin(3)==0)
//			/* Toggle LED1 */
//			key_value = 3;
//		
//		/* Clear the EXTI line 3 pending bit */
//		EXTI_ClearITPendingBit(EXTI_Line3);
//	}
//}

//void EXTI4_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
//	{
//		delay_ms(20);
//		if(PEin(4) == 0)
//			/* Toggle LED1 */
//			key_value = 4;
//		
//		/* Clear the EXTI line 4 pending bit */
//		EXTI_ClearITPendingBit(EXTI_Line4);
//	}
//}
