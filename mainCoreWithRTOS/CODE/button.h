#ifndef _button_h
#define _button_h

#include "headfile.h"

void button_init(void);

extern rt_sem_t key1_sem;
extern rt_sem_t key2_sem;
extern rt_sem_t key3_sem;
extern rt_sem_t key4_sem;
extern rt_sem_t key5_sem;

#endif
