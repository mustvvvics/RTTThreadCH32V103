/*
 * esp8266.h
 *
 *  Created on: Jun 12, 2021
 *      Author: 29275
 */

#ifndef CODE_ESP8266_H_
#define CODE_ESP8266_H_

extern rt_sem_t esp8266_sem;
extern rt_thread_t tidEsp8266;

void Esp_Init(void);
void ESP8266_Clear(void);
void Tcp_Decode(void);
void esp8266Init(void);
void sendFuzzyData(void);

#endif /* CODE_ESP8266_H_ */
