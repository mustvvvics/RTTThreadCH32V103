#ifndef _aaFunction_h
#define _aaFunction_h

#include "headfile.h"

extern uint8 threeWayIn;
extern uint8 threeWaySum;
extern int8 ThreeWayDirection;
extern int32 encoder_x,encoder_y;
extern uint8 encoderCountYFlag;
extern int32 roundIslandMax;
extern uint8 testSideFlag;
extern uint8 encoderCountYFlagMain;

void clearError(void);
void clearFlags(void);
void roundIslandAnalyze(void);
void ThreeWayAnalyze(void);
void encoderCountX(void);
void encoderCountY(void);
void motor_conversion(void);

#endif
