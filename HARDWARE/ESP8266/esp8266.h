#ifndef _ESP8266_H_
#define _ESP8266_H_

#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志

extern unsigned char esp8266_buf[256];
extern unsigned short esp8266_cnt, esp8266_cntPre;

extern void ESP8266_Init(void);

void ESP8266_Clear(void);

extern void ESP8266_SendData(unsigned char *data, unsigned short len);

unsigned char *ESP8266_GetIPD(unsigned short timeOut);


#endif
