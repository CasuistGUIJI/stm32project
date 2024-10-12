#include "adc.h"

void ADC_Config(void)
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;

	/* Enable ADC1  GPIOA clocks */ 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* Configure ADC1 Channel5 pin as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;    //模拟功能：输入的是模拟信号
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ADC Common Init */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;      				// 独立模式
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;   				// 二分频
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;     // 关闭DMA功能
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;// 两次采样间隔时间为5个时钟
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC1 Init */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;   	//12位精度：精度越高转换值越细腻
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;				//关闭扫描
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;			//开启连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//不需要触发
	// ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;      //无需配置 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;     //数据右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;  			   //指定将使用常规通道组的定序器完成的ADC转换次数：完成一次转换
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel5 configuration，3个时钟间隔采样一次，（可以调整采样时间间隔来缩小误差） */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_480Cycles);
	
	/* 清除转换完成标志位 */
	ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
	
	/* Enable ADC3 */
	ADC_Cmd(ADC1, ENABLE);
}

//光敏传感器配置：ADC3
void ADC_photosensitive_Config(void)
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;

	/* Enable ADC1  GPIOA clocks */ 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	/* Configure ADC1 Channel5 pin as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;    //模拟功能：输入的是模拟信号
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* ADC Common Init */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;      				// 独立模式
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;   				// 二分频
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;     // 关闭DMA功能
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;// 两次采样间隔时间为5个时钟
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC3 Init */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;   	//12位精度：精度越高转换值越细腻
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;				//关闭扫描
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;			//开启连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//不需要触发
	// ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;      //无需配置 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;     //数据右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;  			   //指定将使用常规通道组的定序器完成的ADC转换次数：完成一次转换
	ADC_Init(ADC3, &ADC_InitStructure);

	/* ADC3 regular channel5 configuration，3个时钟间隔采样一次，（可以调整采样时间间隔来缩小误差） */
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_480Cycles);
	
	/* 清除转换完成标志位 */
	ADC_ClearFlag(ADC3,ADC_FLAG_EOC);
	
	/* Enable ADC3 */
	ADC_Cmd(ADC3, ENABLE);
	/* 启动第一次转换 */
    ADC_SoftwareStartConv(ADC3);
}

uint16_t ADC_convert(ADC_TypeDef* ADCx,uint32_t ADC_Resolution)
{	
		
	    uint16_t Conv_val, Vol_val;

		/* 启动转换 */ 
		ADC_SoftwareStartConv(ADCx);
		/* 等待转换完成 */
		 while (ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);
	
		ADC_ClearFlag(ADC3,ADC_FLAG_EOC);
		/* 获取转换值 */
		Conv_val = ADC_GetConversionValue(ADCx);
		if(ADC_Resolution == 12){
			/* 通过转换值换算电压值 */
			Vol_val = Conv_val * 3300/4096;//mv
		}else if(ADC_Resolution == 10){
			/* 通过转换值换算电压值 */
			Vol_val = Conv_val * 3300/1024;//mv
		}else if(ADC_Resolution == 8){
			/* 通过转换值换算电压值 */
			Vol_val = Conv_val * 3300/256;//mv
		}else if(ADC_Resolution == 6){
			/* 通过转换值换算电压值 */
			Vol_val = Conv_val * 3300/64;//mv
		}
		
//		printf("转换值:%hd 电压值:%hdmv\r\n", Conv_val, Vol_val);
		
		//如果是配置光敏传感器控制led，需要配置通道这一步转换占空比
//		TIM_SetCompare1(TIM14,Vol_val*100/2800);

		return Conv_val;
}
