#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "bit_band.h"
#include "usart.h"
#include "dht11.h"
#include "ESP8266/esp8266.h"
#include "ONENET/onenet.h"
#include "string.h"
#include "oled.h"
#include "rtc.h"

#include "project.h"
#include "pwm.h"
#include "beep.h"
#include "adc.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "stdio.h"
#include "event_groups.h"

//#include "bmp.h"

//事件标志组
EventGroupHandle_t xEventGroup;

#define INTERTASK (1 << 0) // 内设初始化完成标志
#define EXTERNTASK (1 << 1) // 外设初始化完成标志
#define ONENETTASK (1 << 2) // ONENET初始化连接完成标志

//后续还需要使用可以改
#define LEDWARN (1 << 3)
#define AUTOLED (1 << 4)
#define ESP (1 << 5)
#define CLOUDUP (1 << 6)

u8 humidityH;	  //湿度整数部分
u8 humidityL;	  //湿度小数部分
u8 temperatureH;   //温度整数部分
u8 temperatureL;   //温度小数部分
uint32_t light_value; //光照数值
extern u8 flag;



static TaskHandle_t xCreatTaskHandle = NULL;
//内设任务句柄
static TaskHandle_t xInternalInitTaskHandle = NULL;
//外设任务句柄
static TaskHandle_t xExternalInitTaskHandle = NULL;

//任务句柄
static TaskHandle_t xOneNetTaskHandle = NULL;
static TaskHandle_t xShowTimeTaskHandle = NULL;
static TaskHandle_t xRecordDHT11TaskHandle = NULL;
static TaskHandle_t xHandleLightSensorTaskHandle = NULL;
static TaskHandle_t xOLEDShowDHT11TaskHandle = NULL;
static TaskHandle_t xDHT11WarningTaskHandle = NULL;
static TaskHandle_t xCloudUploadTaskHandle = NULL;

static TaskHandle_t xESP8266TaskHandle = NULL;
// 互斥量句柄
static SemaphoreHandle_t xMutex = NULL;


struct task{
	TaskFunction_t pxTaskCode;
	const char * const pcName; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
	const configSTACK_DEPTH_TYPE usStackDepth;
	void * const pvParameters;
	UBaseType_t uxPriority;
	TaskHandle_t * const pxCreatedTask;
};


void handle_light_sensor(void);
void handle_dht11_warning(DHT11_Data_TypeDef *DHT11_Data);

void vESP8266Task(void *pvParameters)
{
	ESP8266_Init();
//	xEventGroupSetBits(xEventGroup, ESP);
	vTaskDelete(xESP8266TaskHandle);
}
void vInternalInitTask(void *pvParameters) {
    // 初始化内设
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
		

		printf("开始内设初始化... \r\n");
		LED_init();       // LED 初始化
        RTC_INITConfig();    // RTC 初始化
        
        printf("内设初始化完成！！ \r\n");
        xSemaphoreGive(xMutex); // 释放互斥量
    }
	
	// 光敏传感器配置
    ADC_photosensitive_Config(); 
    // LED0 点亮指示
    Led0_Set(LED_ON);

    // 设置事件组标志，通知外设初始化任务
    xEventGroupSetBits(xEventGroup, INTERTASK);

    // 删除内设初始化任务
    vTaskDelete(xInternalInitTaskHandle);
}

void vExternalInitTask(void *pvParameters) {
	printf("等待内设初始化完成... \r\n");
    // 等待事件组标志位，直到内设初始化完成
    xEventGroupWaitBits(xEventGroup, INTERTASK, pdTRUE, pdTRUE, portMAX_DELAY);
	
	printf("开始外设初始化... \r\n");
    // 初始化外设
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
        DHT11_Init();     // DHT11 初始化
        OLED_Init();      // OLED 初始化
        OLED_Clear();     // 清屏
        
        BEEP_init();      // 蜂鸣器初始化
        KEY_init();       // 按键初始化
        ESP8266_Init();  // 初始化 ESP8266
        printf("外设初始化完成 \r\n");
        xSemaphoreGive(xMutex); // 释放互斥量
    }
    
    // LED 点亮指示
    Led1_Set(LED_ON);

    // 外部中断初始化
    EXTI_Config();

    // 读取 DHT11 数据
    DHT11_Data_TypeDef DHT11_Data;
    if (xSemaphoreTake(xMutex, pdMS_TO_TICKS(1000)) == pdTRUE) { // 加入超时处理
        Read_DHT11(&DHT11_Data); // 从 DHT11 读取数据
        xSemaphoreGive(xMutex);  // 释放互斥量
    } else {
        printf("读取 DHT11 数据超时 \r\n");
        // 进行错误处理
    }

    // RTC 初始化
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
        if (RTC_ReadBackupRegister(RTC_BKP_DR3) != 0x7777) {
            RTC_INITConfig();
            RTC_WriteBackupRegister(RTC_BKP_DR3, 0x7777);
        }
        RTC_WakeUp_Config();  // 唤醒定时器配置
        printf("RTC 初始化完成 \r\n");
        xSemaphoreGive(xMutex); // 释放互斥量
    }

    // FLASH 初始化
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
        FLASH_Read(4, 0, (uint8_t *)&msg_num, sizeof msg_num);
        printf("FLASH 初始化完成 \r\n");
        xSemaphoreGive(xMutex); // 释放互斥量
    }

    // 完成所有初始化
    printf("外设和内设初始化完毕!!! \r\n");
	
    Led2_Set(LED_ON);
	
	 // 设置事件组标志，表示外设初始化完成
    xEventGroupSetBits(xEventGroup, EXTERNTASK);
    // 删除外设初始化任务
    vTaskDelete(xExternalInitTaskHandle);
}

// OneNet接入任务
void vOneNetTask(void *pvParameters) {
	 // 等待外设初始化完成
    xEventGroupWaitBits(xEventGroup, EXTERNTASK, pdTRUE, pdTRUE, portMAX_DELAY);
    printf("OneNet 接入中...\r\n");
    // 接入 OneNET
    while(OneNet_DevLink()) {
        delay_ms(500);  // 循环尝试连接
    }
    
    printf("ONENET 接入成功! \r\n");
    OneNET_Subscribe();  // 订阅云平台服务
	delay_ms(500);
    // 操作 LED
    PFout(8) = 1;
    Led3_Set(LED_ON);
    delay_ms(500);

    // 关闭所有 LED 和蜂鸣器
    PFout(8) = 0;
    Led0_Set(LED_OFF);
    Led1_Set(LED_OFF);
    Led2_Set(LED_OFF);
    Led3_Set(LED_OFF);
	xEventGroupSetBits(xEventGroup, ONENETTASK);
    printf("云平台初始化完毕!!!\r\n");
	vTaskDelete(xOneNetTaskHandle);
}

// 云平台上传任务
void vCloudUploadTask(void *pvParameters) {
	xEventGroupWaitBits(xEventGroup, ONENETTASK, pdTRUE, pdTRUE, portMAX_DELAY);
    DHT11_Data_TypeDef DHT11_Data;
    unsigned short timeCount = 0;    // 用于计时上传间隔
    while (1) {
		light_value = ADC_convert(ADC3, 12);
        // 读取传感器数据
        if (++timeCount >= 100) {    // 时间间隔 1s (根据vTaskDelay的时间调整)
            if (Read_DHT11(&DHT11_Data) == SUCCESS) { 
                // 提取温湿度数据
                humidityH = DHT11_Data.humi_int;    // 湿度整数部分
                humidityL = DHT11_Data.humi_deci;   // 湿度小数部分
                temperatureH = DHT11_Data.temp_int; // 温度整数部分
                temperatureL = DHT11_Data.temp_deci;// 温度小数部分
                printf("humi:%d.%d  temp:%d.%d  light:%d\r\n", humidityH, humidityL, temperatureH, temperatureL,light_value);

                // 上传数据到云平台
                OneNet_SendData();
//                printf("Data sent to OneNet!\r\n");

                // 清理 ESP8266 的缓存
                ESP8266_Clear();
            }
            timeCount = 0; // 重置计时器
        }
        // 检查云平台下发的指令
        unsigned char *dataPtr = ESP8266_GetIPD(0); // 获取平台返回的数据
        if (dataPtr != NULL) {
            OneNet_RevPro(dataPtr);  // 处理下发指令
        }

        // 延迟 10 ms，让出 CPU 给其他任务
		delay_ms(10);
//        vTaskDelay(xDelay);
    }
}
// 显示时间的任务
void vShowTimeTask(void *pvParameters) {
	xEventGroupWaitBits(xEventGroup, ONENETTASK, pdTRUE, pdTRUE, portMAX_DELAY);
    for( ;; ) {
		 if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
			showtime(); // 显示时间
			xSemaphoreGive(xMutex); // 释放互斥量
		}
		delay_ms(100);
		
    }
}

// 记录 DHT11 数据的任务(BUG)
void vRecordDHT11Task(void *pvParameters) {
	xEventGroupWaitBits(xEventGroup, ONENETTASK, pdTRUE, pdTRUE, portMAX_DELAY);
    for( ;; ) {
		if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
			record_dht11();
			xSemaphoreGive(xMutex); // 释放互斥量
			delay_ms(100);
		}
    }
}

// 处理光传感器的任务(有问题)
void vHandleLightSensorTask(void *pvParameters) {
	xEventGroupWaitBits(xEventGroup, ONENETTASK, pdTRUE, pdTRUE, portMAX_DELAY);
    for( ;; ) {
		handle_light_sensor();
		delay_ms(100);
    }
}


// 显示 DHT11 数据的任务(和时间显示相互冲突)
void vOLEDShowDHT11Task(void *pvParameters) {
	xEventGroupWaitBits(xEventGroup, ONENETTASK, pdTRUE, pdTRUE, portMAX_DELAY);
    for( ;; ) {
       if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
            OLED_Show_dht11(); // 显示 DHT11 数据
            xSemaphoreGive(xMutex); // 释放互斥量
        }
		delay_ms(100);
    }
}

// 处理 DHT11 警告的任务
void vDHT11WarningTask(void *pvParameters) {
	xEventGroupWaitBits(xEventGroup, ONENETTASK, pdTRUE, pdTRUE, portMAX_DELAY);
    DHT11_Data_TypeDef DHT11_Data;
    for( ;; ) {
		if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
			Read_DHT11(&DHT11_Data);
			handle_dht11_warning(&DHT11_Data);
			xSemaphoreGive(xMutex); // 释放互斥量
		}
		delay_ms(100);
    }
}

struct task task_arry[] = {
	{vESP8266Task, "ESP8266", 256, NULL, 8, &xESP8266TaskHandle},
	{vInternalInitTask, "InternalInit", 512, NULL, 9, &xInternalInitTaskHandle },
    {vExternalInitTask, "ExternalInitTask", 512, NULL, 8, &xExternalInitTaskHandle },
    {vOneNetTask, "OneNetTask", 512, NULL, 7, &xOneNetTaskHandle } ,
	{vShowTimeTask, "ShowTimeTask", 512, NULL, 3, &xShowTimeTaskHandle},
	{vRecordDHT11Task, "RecordDHT11Task", 512, NULL, 4, &xRecordDHT11TaskHandle},
	{vHandleLightSensorTask, "HandleLightSensorTask", 512, NULL, 4, &xHandleLightSensorTaskHandle},
//	{vOLEDShowDHT11Task, "OLEDShowDHT11Task", 512, NULL, 4, &xOLEDShowDHT11TaskHandle},
	{vDHT11WarningTask, "DHT11WarningTask", 512, NULL, 4, &xDHT11WarningTaskHandle},
	{vCloudUploadTask, "CloudUploadTask", 512, NULL, 5, &xCloudUploadTaskHandle}
};
void vCreatTask( void * pvParameters )
{
	u8 i =0;
	printf("%s is running..\r\n",__FUNCTION__);
	while(task_arry[i].usStackDepth)
	{
		xTaskCreate(task_arry[i].pxTaskCode,
					task_arry[i].pcName,
					task_arry[i].usStackDepth,
					task_arry[i].pvParameters,
					task_arry[i].uxPriority,
					task_arry[i].pxCreatedTask
		);
		i++;
		delay_ms(1000);//INCLUDE_vTaskDelay
	}
	vTaskDelete(xCreatTaskHandle);
}


int main(void) {
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	USART1_init(115200); // 初始化 USART1
	USART3_init(115200); // 初始化 USART3
	
    // 创建互斥量
    xMutex = xSemaphoreCreateMutex();
    if(xMutex == NULL) {
        // 互斥量创建失败处理
        printf("互斥量创建失败\n");
        while(1);
    }

	// 创建事件组
    xEventGroup = xEventGroupCreate();
//	xTaskCreate(vESP8266Task, "ESP8266", 128, NULL, 9, &xESP8266TaskHandle);
	
//	xTaskCreate(vCreatTask,
//				"INIT_ALL_DEV",
//				512,
//				NULL,
//				9,
//				&xCreatTaskHandle
//	);
	

//    // 创建任务
    xTaskCreate(vInternalInitTask, "InternalInit", 512, NULL, 9, &xInternalInitTaskHandle);
    xTaskCreate(vExternalInitTask, "ExternalInitTask", 512, NULL, 8, &xExternalInitTaskHandle);
	// 创建 OneNet 连接相关任务
    xTaskCreate(vOneNetTask, "OneNetTask", 512, NULL, 7, &xOneNetTaskHandle);
	
//	实时时间
	xTaskCreate(vShowTimeTask, "ShowTimeTask", 128, NULL, 3, &xShowTimeTaskHandle);
//	记录温湿度控制
    xTaskCreate(vRecordDHT11Task, "RecordDHT11Task", 512, NULL, 4, &xRecordDHT11TaskHandle);
//	光照自动控制
    xTaskCreate(vHandleLightSensorTask, "HandleLightSensorTask", 128, NULL, 4, &xHandleLightSensorTaskHandle);
//	能显示，但是不能并行运行
//    xTaskCreate(vOLEDShowDHT11Task, "OLEDShowDHT11Task", 512, NULL, 4, &xOLEDShowDHT11TaskHandle);

//	温湿度阈值报警
    xTaskCreate(vDHT11WarningTask, "DHT11WarningTask", 512, NULL, 4, &xDHT11WarningTaskHandle);
	// 创建云平台上传任务
    xTaskCreate(vCloudUploadTask, "CloudUploadTask", 512, NULL, 5, &xCloudUploadTaskHandle);
    // 启动调度器
    vTaskStartScheduler();
	
}


void handle_light_sensor(void) {

	uint16_t light_value = ADC_convert(ADC3, 12);

    if (light_value >= 3000) {
        LED1 = 0;  // 开灯
        printf("检测到亮度过低，已自动打开灯光... \r\n");
	}else{
		LED1 = 1;
	}
}


void handle_dht11_warning(DHT11_Data_TypeDef *DHT11_Data) {
	u8 temp,humi;
	temp = DHT11_Data->temp_int;
	humi = DHT11_Data->humi_int;
	printf("温湿度监测中....(temp:%d  humi:%d)\r\n",temp,humi);
    // 温湿度超过阈值报警
    if (temp >= 70 && temp >= 27) {
        led_warning_th();
		printf("检测到温度:%d,湿度:%d均超过阈值！！！\r\n",DHT11_Data->temp_int,DHT11_Data->humi_int);
    } else if (humi >= 70) {
        led_warning_h();
		printf("检测到》》湿度:%d《《超过阈值！！！\r\n",DHT11_Data->humi_int);
    } else if (temp >= 29) {
        led_warning_t();
		printf("检测到》》温度:%d《《超过阈值！！！\r\n",DHT11_Data->temp_int);
    }
}



/* Task to be created. */
//void vTaskUSART1_3( void * pvParameters )
//{
//	uint16_t temp,humi;
//    const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
//    for( ;; )
//    {
//		dth11_read_data(&temp,&humi);
//        printf("温度：%d  湿度：%d \r\n",temp,humi);
////		vTaskDelay(xDelay);//INCLUDE_vTaskDelay
//		delay_s(1);
//    }
//}

//void vTaskHandWare( void * pvParameters )
//{
//    const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
//    for( ;; )
//    {
//        printf("%s is running..\r\n",__FUNCTION__);
//		vTaskDelay(xDelay);//INCLUDE_vTaskDelay
//    }
//}


void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/
void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/
void vApplicationTickHook( void )
{
	
}
/*-----------------------------------------------------------*/
