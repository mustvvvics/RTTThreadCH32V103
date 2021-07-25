#ifndef _display_h
#define _display_h

#include "headfile.h"

extern rt_thread_t tidDisplay;
extern uint8 mainWriteFlashFlag;
extern uint8 confirmButton;
extern uint8 carStart;
extern uint8 clearCamFlags;
extern int32 elementTable; //元素顺序表
extern uint8 elementTableLength;
extern uint8 drivingDirectionToCam;

/******************Threewayroad*****************************************/
extern uint8 fixCamThreewayFeatureRow;// +- 1
/******************Crossroad********************************************/
extern uint8 fixCamDetectCrossroadMissingNumThres;//+- 1
/******************SharpCurve*******************************************/
extern uint8 fixCamRangeSharpCurveRow;//+- 1
//int16 fixCamSharpCurveJitterThres = unknown;
//int32 fixCamBigCurveThres = unknown;
/******************Else*************************************************/
extern int8 fixCamGlobalCenterBias;//+- 1
//int32 fixCamOutboundAreaThres = unknown;//+- 2000
extern uint16 fixCamStartlineJumpingPointNumThres;//+- 10
/*
 * page 3 in cam display
 */
/******************Basic************************************************/
extern uint8 fixCamDetectDistance;//+- 1 实际为float 2.0 +- 0.1
extern int32 fixCamPixelMeanThres;//+- 10
extern uint8 fixCamSlopeRowStart;//+- 1
extern uint8 fixCamSlopeRowEnd;//+- 1
/******************Roundabout*******************************************/
extern int32 fixCamAreaDetectRoundaboutThresLeft;//+- 10
extern int32 fixCamAreaDetectRoundaboutThresRight;//+- 10
extern uint8 fixCamRoundaboutDetectionStartRow;//+- 1


void display_init(void);

#endif
