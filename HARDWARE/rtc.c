#include "rtc.h"

#define RTC_CLOCK_SOURCE_LSE  // 定义RTC时钟源为LSE（外部低速晶振）
RTC_TimeTypeDef RTC_TimeStructure;  // RTC时间结构体
RTC_DateTypeDef RTC_DateStructure;  // RTC日期结构体

volatile uint32_t uwAsynchPrediv = 0;  // 异步预分频器值
volatile uint32_t uwSynchPrediv = 0;   // 同步预分频器值
volatile u8 RTC_Wakeup_event_flag = 0; // 唤醒事件标志

/**
 * @brief 初始化RTC时钟
 */
void RTC_INITConfig(void)
{
    RTC_InitTypeDef RTC_InitStructure;  // RTC初始化结构体
    
    /* 启用PWR（电源）时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* 允许访问RTC寄存器 */
    PWR_BackupAccessCmd(ENABLE);

    // 判断是否需要重新初始化RTC配置
//  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x8888)
//  {
        #if defined (RTC_CLOCK_SOURCE_LSI)  /* 使用内部低速时钟LSI作为RTC时钟源 */
        /* RTC时钟频率可能会随LSI频率漂移 */
        /* 启用LSI振荡器 */
        RCC_LSICmd(ENABLE);

        /* 等待LSI时钟就绪 */
        while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
        {
        }

        /* 选择LSI作为RTC时钟源 */
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

        /* 设置RTC异步和同步预分频器 */
        uwSynchPrediv = 0x7F;
        uwAsynchPrediv = 0xF9;

        #elif defined (RTC_CLOCK_SOURCE_LSE) /* 使用LSE作为RTC时钟源 */
        /* 启用LSE振荡器 */
        RCC_LSEConfig(RCC_LSE_ON);

        /* 等待LSE时钟就绪 */
        while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        {
        }

        /* 选择LSE作为RTC时钟源 */
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        /* 设置RTC异步和同步预分频器 */
        uwSynchPrediv = 0xFF;
        uwAsynchPrediv = 0x7F;

        #else
        #error 请在main.c文件中选择RTC时钟源
        #endif /* RTC_CLOCK_SOURCE_LSI */

        /* 启用RTC时钟 */
        RCC_RTCCLKCmd(ENABLE);

        /* 等待RTC寄存器同步 */
        RTC_WaitForSynchro();

        /* 配置RTC初始化参数 */
        RTC_InitStructure.RTC_AsynchPrediv = uwAsynchPrediv;
        RTC_InitStructure.RTC_SynchPrediv = uwSynchPrediv;
        RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;  // 24小时制
        RTC_Init(&RTC_InitStructure);

        /* 设置日期：2024年9月27日，星期五 */
        RTC_DateStructure.RTC_Year = 0x24;   // 年
        RTC_DateStructure.RTC_Month = RTC_Month_September; // 月
        RTC_DateStructure.RTC_Date = 0x27;   // 日
        RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Friday;  // 星期
        RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

        /* 设置时间：上午9点10分00秒 */
        RTC_TimeStructure.RTC_H12 = RTC_H12_PM;  // AM/PM
        RTC_TimeStructure.RTC_Hours = 0x15;      // 小时
        RTC_TimeStructure.RTC_Minutes = 0x59;    // 分钟
        RTC_TimeStructure.RTC_Seconds = 0x55;    // 秒
        RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);   

        /* 设置RTC已配置标志（注释掉了，不会写入备份寄存器） */
//      RTC_WriteBackupRegister(RTC_BKP_DR0, 0x8888);
//  }
		printf("RTC_INITConfig_INIT_Success \r\n");
}

/**
 * @brief 配置RTC闹钟A
 */
void RTC_Alarm_Config(void)
{
    RTC_AlarmTypeDef RTC_AlarmStructure; // RTC闹钟结构体
    EXTI_InitTypeDef EXTI_InitStructure; // 外部中断结构体
    NVIC_InitTypeDef NVIC_InitStructure; // 中断控制器结构体

    /* 允许访问RTC寄存器 */
    PWR_BackupAccessCmd(ENABLE);

    /* 设置闹钟时间：上午9点10分05秒 */
    RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM; // AM/PM
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = 0x09;     // 小时
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x10;   // 分钟
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x05;   // 秒

    /* 设置闹钟日期为星期五 */
    RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_Weekday_Friday;
    RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;
    RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_None; // 不屏蔽任何字段

    /* 关闭闹钟A */
    RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
    
    /* 设置闹钟A */
    RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);

    /* 启用闹钟A */
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

    /* 清除RTC闹钟A标志 */
    RTC_ClearFlag(RTC_FLAG_ALRAF);
    RTC_ClearITPendingBit(RTC_IT_ALRA);

    /* 配置外部中断线17，用于RTC闹钟 */
    EXTI_ClearITPendingBit(EXTI_Line17);
    EXTI_InitStructure.EXTI_Line = EXTI_Line17;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* 配置中断优先级 */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief RTC闹钟中断处理函数
 */
void RTC_Alarm_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_ALRA) != RESET)
    {
        /* 清除RTC闹钟A标志 */
        RTC_ClearITPendingBit(RTC_IT_ALRA);

        /* 清除外部中断线17标志 */
        EXTI_ClearITPendingBit(EXTI_Line17);

        /* 用户自定义处理代码 */
        printf("RTC AlarmA Triggered!\r\n");
		
		
    }
}

/**
 * @brief 显示当前RTC时间
 */
void RTC_TimeShow(void)
{
    /* 获取当前时间 */
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

    /* 显示时间格式：年月日 星期 时分秒 */
    printf("20%0.2d/%0.2d/%0.2d %0.2d\t", RTC_DateStructure.RTC_Year, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_WeekDay);
    printf("%0.2d:%0.2d:%0.2d\r\n", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
}

/**
 * @brief 配置RTC唤醒功能
 */
void RTC_WakeUp_Config(void)
{
	
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	/* Allow access to RTC 解除保护，允许访问RTC的寄存器 */
	PWR_BackupAccessCmd(ENABLE);
	
	// 关闭唤醒功能
	RTC_WakeUpCmd(DISABLE);
	
	// 16为唤醒寄存器1Hz时钟源
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	
	// 设置唤醒寄存器计数值(递减到0则触发)
	RTC_SetWakeUpCounter(0); // 计数一次等于时间1s
	
	//清除RTC中断标志WUTE
	RTC_ClearITPendingBit(RTC_IT_WUT);
	// 清除线22
	EXTI_ClearITPendingBit(EXTI_Line22);
	
	// 使能唤醒中断
	RTC_ITConfig(RTC_IT_WUT, ENABLE);
	
	// 开启唤醒功能
	RTC_WakeUpCmd(ENABLE);
	
	// 配置线22上升沿触发
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	// 配置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;            // 指定配置中断通道：串口2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;    // 指定中断通道的抢占优先级：最高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           // 指定中断通道的响应优先级：最高
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 // 指定中断通道触发信号状态：使能
	NVIC_Init(&NVIC_InitStructure); // 按照结构体的初始化去配置硬件
	printf("RTC_INITConfig_INIT_END \r\n");
}

void RTC_WKUP_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
	{
		//用户代码
		RTC_Wakeup_event_flag = 1;
		
		//清除中断标志
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
	}
}


/**
 * @brief 显示唤醒后的时间
 */
void wakeup_show(void)
{
    if (RTC_Wakeup_event_flag == 1)
    {
        RTC_Wakeup_event_flag = 0;

        /* 获取并显示当前时间 */
        RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
        RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
        printf("20%0.2d/%0.2d/%0.2d %0.2d\t", RTC_DateStructure.RTC_Year, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_WeekDay);
        printf("%0.2d:%0.2d:%0.2d\r\n", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
    }
}
