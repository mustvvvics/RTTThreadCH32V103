/*
 * esp8266.h
 *
 *  Created on: Jun 12, 2021
 *      Author: 29275
 */

#ifndef CODE_ESP8266_H_
#define CODE_ESP8266_H_


#define REV_OK      0
#define REV_WAIT    1

extern rt_sem_t esp8266_sem;
extern rt_sem_t control_sem;
void clearError(void);
void Esp_Init(void);
void ESP8266_Clear(void);
void Tcp_Decode(void);
void sendMessage(void);
void esp8266Init(void);
void Esp_IP_Get(uint8 data_temp);
void manual_control(void);

#endif /* CODE_ESP8266_H_ */
