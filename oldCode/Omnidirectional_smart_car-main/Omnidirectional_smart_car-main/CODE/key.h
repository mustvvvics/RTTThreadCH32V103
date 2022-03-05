/*
 * key.h
 *
 *  Created on: Jan 13, 2021
 *      Author: xm2000
 */

#ifndef CODE_KEY_H_
#define CODE_KEY_H_

#include "headfile.h"
//定义按键引脚
#define KEY1    B2
#define KEY2    B4
#define KEY3    B5

uint8 key_scan(void);

#endif /* CODE_KEY_H_ */
