#include "project.h"

static uint8_t cmdflag = 0;  // 命令状态标志
uint8_t cmd = 0;             // 当前命令
uint16_t temp, humi;         // 温湿度值
uint32_t msg_num = 0;       // 消息计数
char buf[MSG_LEN];           // 缓冲区

void record_dht11(void)
{
    DHT11_Data_TypeDef DHT11_Data;
    RTC_DateTypeDef RTC_DateStructure;
    RTC_TimeTypeDef RTC_TimeStructure;

    // 检查 USART 接收标志或者按键值
    if (usart1_recv_flag || key_value)
    {
        // 检查命令
        if (strcmp((char *)usart1_recv_data, "start") == 0 || key_value == 1)
        {
            if (cmdflag == 2)
            {
                // 如果之前是停止状态，继续记录
                printf("继续记录温湿度..\r\n");
                cmd = START;
                cmdflag = 0;
                OLED_Clear(); // 清屏
                OLED_ShowCHinese(28, 2, 10); // 显示“继续记录温湿度”
				OLED_ShowCHinese(43, 2, 12);
				OLED_ShowCHinese(58, 2, 11);
				OLED_ShowCHinese(73, 2, 25);
				OLED_ShowCHinese(88, 2, 26);
				OLED_ShowCHinese(43, 4, 30);
				OLED_ShowCHinese(58, 4, 31);
				OLED_ShowChar(66,4,'.',16);
				OLED_ShowChar(72,4,'.',16);
				
                delay_ms(500); // 延时500ms
            }
            else
            {
                // 第一次开始记录
                printf("开始记录温湿度..\r\n");
                cmd = START;
                cmdflag = 0;
                OLED_Clear(); // 清屏

                OLED_ShowCHinese(28, 2, 10);  // 显示“开始记录温湿度”
				OLED_ShowCHinese(43, 2, 12);
				OLED_ShowCHinese(58, 2, 11);
				OLED_ShowCHinese(73, 2, 25);
				OLED_ShowCHinese(88, 2, 26);
				OLED_ShowCHinese(43, 4, 27);
				OLED_ShowCHinese(58, 4, 28);
				OLED_ShowCHinese(73, 4, 29);
                delay_ms(500); // 延时500ms
            }
        }
        else if (strcmp((char *)usart1_recv_data, "stop") == 0 || key_value == 2)
        {
            printf("停止记录温湿度..\r\n");
            cmd = STOP;
			OLED_Clear(); // 清屏
			delay_ms(100);
            
        }
        else if (strcmp((char *)usart1_recv_data, "show") == 0 || key_value == 3)
        {
            printf("已记录的温湿度数据：\r\n");
            cmd = SHOW;
        }
        else if (strcmp((char *)usart1_recv_data, "clear") == 0 || key_value == 4)
        {
            printf("已清除全部温湿度数据 \r\n");
			
            cmd = CLEAR;
			OLED_Clear(); // 清屏
			
        }
        key_value = 0; // 重置按键值
        usart1_recv_flag = false; // 重置接收标志
    }

    switch (cmd)
    {
        case START:
            if (RTC_Wakeup_event_flag)
            {
                RTC_Wakeup_event_flag = 0; // 重置 RTC 唤醒标志
                if (msg_num < 100)
                {
                    // 读取温湿度数据
                    Read_DHT11(&DHT11_Data);
                    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
                    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

                    // 每次更新温湿度数据时不清屏
//					OLED_Show_dht11();
                    // 记录数据到闪存
                    memset(buf, 0, MSG_LEN);
                    sprintf(buf, "[%03d]20%0.2d/%0.2d/%d %0.2d:%0.2d:%0.2d Temp=%d.0 Humi=%d.0\r\n",
                            msg_num + 1, RTC_DateStructure.RTC_Year, RTC_DateStructure.RTC_Month,
                            RTC_DateStructure.RTC_Date, RTC_TimeStructure.RTC_Hours,
                            RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds,
                            DHT11_Data.temp_int, DHT11_Data.humi_int);
                    printf("%s", buf);
					
					// 在 LED 屏幕上显示温度和湿度
					Display_On_LED_Screen(DHT11_Data.temp_int, DHT11_Data.humi_int);
					
                    FLASH_Write(6, msg_num * MSG_LEN, (uint8_t *)buf, MSG_LEN);
                    msg_num++;
                    FLASH_Write(4, 0, (uint8_t *)&msg_num, sizeof msg_num);
                }
                else
                {
                    printf("存储已满\r\n");
                }
            }
            break;

        case STOP:
			
			OLED_ShowCHinese(28, 2, 10);  // 显示“停止记录温湿度”
			OLED_ShowCHinese(43, 2, 12);
			OLED_ShowCHinese(58, 2, 11);
			OLED_ShowCHinese(73, 2, 25);
			OLED_ShowCHinese(88, 2, 26);
			
			OLED_ShowCHinese(43, 4, 27);
			OLED_ShowCHinese(58, 4, 32);
			OLED_ShowCHinese(73, 4, 33);
            cmd = 0; // 重置命令
			cmdflag = 2; // 更新状态
            break;

        case SHOW:
            cmd = 0; // 重置命令
			cmdflag = cmdflag; // 更新状态
            for (int i = 0; i < msg_num; i++)
            {
                memset(buf, 0, MSG_LEN);
                FLASH_Read(6, i * MSG_LEN, (uint8_t *)buf, MSG_LEN);
                printf("%s", buf);
            }
            break;

        case CLEAR:
//			OLED_Clear(); // 清屏
			OLED_ShowCHinese(28, 2, 10);  // 显示“开始记录温湿度”
			OLED_ShowCHinese(43, 2, 12);
			OLED_ShowCHinese(58, 2, 11);
			OLED_ShowCHinese(73, 2, 25);
			OLED_ShowCHinese(88, 2, 26);
			
			OLED_ShowCHinese(43, 4, 27);
			OLED_ShowCHinese(58, 4, 34);
			OLED_ShowCHinese(73, 4, 35);
			
			cmdflag = 0; // 更新状态
            cmd = 0; // 重置命令
            FLASH_Init(6);
            msg_num = 0;
            FLASH_Write(4, 0, (uint8_t *)&msg_num, sizeof msg_num);
            break;

        default:
            break;
    }
	
}


