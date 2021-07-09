

#ifndef CODE_ESP8266CAM_H_
#define CODE_ESP8266CAM_H_

#include "headfile.h"

#define REV_OK      0
#define REV_WAIT    1

extern rt_sem_t esp8266_sem;

void Esp_Init(void);
void ESP8266_Clear(void);
void Tcp_Decode(void);
void sendMessage(void);
void esp8266Init(void);

#endif /* CODE_ESP8266CAM_H_ */