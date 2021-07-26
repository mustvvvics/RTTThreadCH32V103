#ifndef _button_h
#define _button_h

#include "headfile.h"

void button_init(void);
extern rt_mailbox_t key_mailbox;
extern uint32 key_data;

#endif
