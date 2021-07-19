#ifndef CODE_ESP8266CAM_H_
#define CODE_ESP8266CAM_H_

#include "headfile.h"

extern rt_sem_t esp8266_sem;
extern int32 Int2Float;

void Esp_Init(void);
void ESP8266_Clear(void);
void Tcp_Decode(void);
void sendMessage(void);
void esp8266Init(void);
void sendMessageData(void);

#endif /* CODE_ESP8266CAM_H_ */
