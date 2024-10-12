/**
	**********************************************************************
	**********************************************************************
	**********************************************************************
	*	文件名： 	onenet.c
	*	作者： 		zh
	*	日期： 		
	*	版本： 		V1.1
	*	说明： 		与onenet平台的数据交互接口层
	*	修改记录：	
	*	
	**********************************************************************
	**********************************************************************
	**********************************************************************
**/

//单片机头文件
#include "stm32f4xx.h"

//网络设备
#include "ESP8266/esp8266.h"

//协议文件
#include "ONENET/onenet.h"
#include "MQTT/mqttkit.h"

//硬件驱动
#include "usart.h"
#include "delay.h"
#include "DHT11.h"

//C库
#include <string.h>
#include <stdio.h>

#define PROID		"0vF205coZ6"  //产品ID
					//version=2018-10-31&res=products%2FAL771S2CvP%2Fdevices%2Ftemp1&et=2042935266000&method=md5&sign=%2FQe320UEJOEt%2Br4U9AJMOw%3D%3D
#define AUTH_INFO	"version=2018-10-31&res=products%2F0vF205coZ6%2Fdevices%2FEnvironmental_Monitoring&et=3085401600&method=md5&sign=lxhP9OXvonjZt7Ox2%2Fbteg%3D%3D"  //鉴权信息

#define DEVID		"Environmental_Monitoring"   //设备名称

extern unsigned char esp8266_buf[256];
u8 flag;
//==========================================================
//	函数名称：	OneNet_DevLink
//
//	函数功能：	与onenet创建连接
//
//	入口参数：	无
//
//	返回参数：	1-成功	0-失败
//
//	说明：		与onenet平台建立连接
//==========================================================
_Bool OneNet_DevLink(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//协议包

	unsigned char *dataPtr;
	
	_Bool status = 1;
	
	if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 1, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)  //修改clean_session=1
	{
		
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);	//上传平台
		
		dataPtr = ESP8266_GetIPD(250);	//等待平台响应
		if(dataPtr != NULL)
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
			{
				switch(MQTT_UnPacketConnectAck(dataPtr))
				{
					case 0:printf("TIPS:连接成功!\r\n");status = 0;break;
					
					case 1:printf("1\r\n");break;
					case 2:printf("2\r\n");break;
					case 3:printf("3\r\n");break;
					case 4:printf("4\r\n");break;
					case 5:printf("5\r\n");break;
					
					default:break;
				}
			}
		}
		
		MQTT_DeleteBuffer(&mqttPacket);		//删包
	}
	
	return status;
	
}


//访问ONENET需要提交JSON数据，就获取到的温湿度转换为JSON数据形式
extern u8 temperatureH,humidityH;
extern u32 light_value;
unsigned char OneNet_FillBuf(char *buf)
{
	
	char text[48];
	
	memset(text, 0, sizeof(text));
	
	strcpy(buf, "{\"id\":\"123\",\"params\":{");
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"temp\":{\"value\":%d},", temperatureH);
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"humi\":{\"value\":%d},", humidityH);
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"light\":{\"value\":%d},", light_value);
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"led0\":{\"value\":%s}", led_info.Led0_Status ? "true" : "false");
	strcat(buf, text);
//	
	strcat(buf, "}}");
	
	return strlen(buf);

}



//==========================================================
//	函数名称：	OneNet_SendData
//
//	函数功能：	上传数据到平台
//
//	入口参数：	type：发送数据的格式
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_SendData(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};		//协议包
	
	char buf[256];
	short body_len = 0, i = 0;
//	printf("Tips:OneNet_SendData-MQTT\r\n");
	memset(buf, 0, sizeof(buf));    //清空buff
	body_len=OneNet_FillBuf(buf);	
	
	if(body_len)
	{
		
		if(MQTT_PacketSaveData(DEVID, body_len, NULL, 5, &mqttPacket) == 0)							//封包
		{
			
			for(; i < body_len; i++)
				mqttPacket._data[mqttPacket._len++] = buf[i];
			
			ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//上传数据到平台
//			printf("Send %d Bytes\r\n", mqttPacket._len);
			MQTT_DeleteBuffer(&mqttPacket);															//删包
		}else
		printf("WARN:	EDP_NewBuffer Failed\r\n");
	}
	
}




//==========================================================
//	函数名称：	OneNET_Publish
//
//	函数功能：	发布消息
//
//	入口参数：	topic：发布的主题
//				msg：消息内容
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNET_Publish(const char *topic, const char *msg)
{

	MQTT_PACKET_STRUCTURE mqtt_packet = {NULL, 0, 0, 0};						//协议包
	
	printf("Publish Topic: %s, Msg: %s\r\n", topic, msg);
	
	if(MQTT_PacketPublish(MQTT_PUBLISH_ID, topic, msg, strlen(msg), MQTT_QOS_LEVEL0, 0, 1, &mqtt_packet) == 0)
	{
		ESP8266_SendData(mqtt_packet._data, mqtt_packet._len);					//向平台发送订阅请求
		
		MQTT_DeleteBuffer(&mqtt_packet);										//删包
	}

}

//==========================================================
//	函数名称：	OneNET_Subscribe
//
//	函数功能：	订阅
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNET_Subscribe(void)
{
	
	MQTT_PACKET_STRUCTURE mqtt_packet = {NULL, 0, 0, 0};						//协议包
	
	char topic_buf[84];
	const char *topic = topic_buf;
	
	snprintf(topic_buf, sizeof(topic_buf), "$sys/0vF205coZ6/Environmental_Monitoring/thing/property/set");
	
	printf("Subscribe Topic: %s\r\n", topic_buf);
	
	if(MQTT_PacketSubscribe(MQTT_SUBSCRIBE_ID, MQTT_QOS_LEVEL0, &topic, 1, &mqtt_packet) == 0)
	{
		ESP8266_SendData(mqtt_packet._data, mqtt_packet._len);					//向平台发送订阅请求
		
		MQTT_DeleteBuffer(&mqtt_packet);										//删包
	}

}


//==========================================================
//	函数名称：	OneNet_RevPro
//
//	函数功能：	平台返回数据检测
//
//	入口参数：	dataPtr：平台返回的数据
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_RevPro(unsigned char *cmd)
{
	
	char *req_payload = NULL;
	char *cmdid_topic = NULL;
	
	unsigned short topic_len = 0;
	unsigned short req_len = 0;
	
	unsigned char qos = 0;
	static unsigned short pkt_id = 0;
	
	unsigned char type = 0;
	
	short result = 0;

	char *dataPtr = NULL;
	char numBuf[10];
	int num = 0;
	
	cJSON *raw_json, *params_json, *led0_json,*led1_json,*led2_json,*led3_json;
	
	type = MQTT_UnPacketRecv(cmd);
//	printf("type:%s\r\n",type);
	switch(type)
	{
		case MQTT_PKT_PUBLISH:																//接收的Publish消息
		
			result = MQTT_UnPacketPublish(cmd, &cmdid_topic, &topic_len, &req_payload, &req_len, &qos, &pkt_id);
			if(result == 0)
			{
				char *data_ptr = NULL;
				
				printf("topic: %s, topic_len: %d, payload: %s, payload_len: %d\r\n",
																	cmdid_topic, topic_len, req_payload, req_len);
				
				raw_json = cJSON_Parse(req_payload);
				params_json = cJSON_GetObjectItem(raw_json,"params");
				led0_json = cJSON_GetObjectItem(params_json,"led0");
				led1_json = cJSON_GetObjectItem(params_json,"led1");
				led2_json = cJSON_GetObjectItem(params_json,"led2");
				led3_json = cJSON_GetObjectItem(params_json,"led3");

				if(led0_json != NULL)
				{
					if(led0_json->type == cJSON_True) Led0_Set(LED_ON);
					else Led0_Set(LED_OFF);
				}
				if(led1_json != NULL)
				{
					if(led1_json->type == cJSON_True) Led1_Set(LED_ON);
					else Led1_Set(LED_OFF);
				}
				if(led2_json != NULL)
				{
					if(led2_json->type == cJSON_True) Led2_Set(LED_ON);
					else Led2_Set(LED_OFF);
				}
				if(led3_json != NULL)
				{
					if(led3_json->type == cJSON_True) Led3_Set(LED_ON);
					else Led3_Set(LED_OFF);
				}
				cJSON_Delete(raw_json);
				
			}
			break;
		case MQTT_PKT_PUBACK:														//发送Publish消息，平台回复的Ack
		
			if(MQTT_UnPacketPublishAck(cmd) == 0)
				printf("Tips:	MQTT Publish Send OK\r\n");
			
		break;
		
		case MQTT_PKT_SUBACK:																//发送Subscribe消息的Ack
		
			if(MQTT_UnPacketSubscribe(cmd) == 0)
				printf("Tips:	MQTT Subscribe OK\r\n");
			else
				printf("Tips:	MQTT Subscribe Err\r\n");
		
		break;
		
		default:
			result = -1;
		break;
	}
	
	ESP8266_Clear();									//清空缓存
	
	if(result == -1)
		return;
	
	if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}

}










//void OneNet_RevPro(unsigned char *cmd)
//{
//	
//	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//协议包
//	
//	char *req_payload = NULL;
//	char *cmdid_topic = NULL;
//	
//	unsigned short topic_len = 0;
//	unsigned short req_len = 0;
//	
//	unsigned char type = 0;
//	
//	short result = 0;

//	char *dataPtr = NULL;
//	char numBuf[10];
//	int num = 0;
//	if(MQTT_PacketPublish(MQTT_PUBLISH_ID, "$sys/0vF205coZ6/Environmental_Monitoring/thing/property/set", NULL, 5, MQTT_QOS_LEVEL0, 0, 1, &mqttPacket) == 0)  //Publish消息
//	{
//		type = MQTT_UnPacketRecv(cmd);
//		switch(type)
//		{
//			case MQTT_PKT_CMD:		//命令下发
//				
//				result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len);	//解出topic和消息体
//				printf("topic: %s, topic_len: %d, payload: %s, payload_len: %d\r\n",cmdid_topic, topic_len, req_payload, req_len);
//			if(result == 0)
//				{
//					if(MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0)	//命令回复组包
//					{	
//						ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//回复命令
//						MQTT_DeleteBuffer(&mqttPacket);			//删包
//					}
//				}
//			
//				break;
//				
//			case MQTT_PKT_PUBACK:			//发送Publish消息，平台回复的Ack
//				
//				break;
//			
//			default:
//				result = -1;
//				break;
//		}
//	}
//	ESP8266_Clear();									//清空缓存
//	
//	if(result == -1)
//		return;
//	
//	dataPtr = strchr(req_payload, '}');					//搜索'}'

////	if(dataPtr != NULL && result != -1)					//如果找到了
////	{
////		dataPtr++;
////		
////		while(*dataPtr >= '0' && *dataPtr <= '9')		//判断是否是下发的命令控制数据
////		{
////			numBuf[num++] = *dataPtr++;
////		}
////		
////		num = atoi((const char *)numBuf);				//转为数值形式
////	}

//	if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
//	{
//		MQTT_FreeBuffer(cmdid_topic);
//		MQTT_FreeBuffer(req_payload);
//	}

//}
