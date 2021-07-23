#ifndef _display_h
#define _display_h

#include "headfile.h"

extern rt_thread_t tidDisplay;
extern uint8 mainWriteFlashFlag;
void display_init(void);

#endif
