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

void Esp_Init(void);
void ESP8266_Clear(void);
void Esp_IP_Get(uint8 data_temp);
void Tcp_Decode(void);


#endif /* CODE_ESP8266_H_ */
