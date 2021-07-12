#ifndef _laneCommon_h
#define _laneCommon_h

#include "headfile.h"

// laneBinarize
//#define uint8 unsigned char
//#define uint16 unsigned short
//#define uint32 unsigned int
//#define uint64 unsigned long
//
//#define int8 char
//#define int32 int
//#define int16 short

#define imgRow 50
#define imgCol 188
//typedef uint8 Mat[imgRow][imgCol];

#define max(x,y) ((x) > (y) ? (x) : (y))
#define min(x,y) ((x) < (y) ? (x) : (y))

#define rangeBiNear 10

// laneAnalyze
extern int32 laneLocationLeft[imgRow];
extern int32 laneLocationRight[imgRow];

// lane center
#define laneCenterStartBias 4

extern int32 laneCenter[imgRow];
extern int32 laneCenterPrevious;

// lane detection flag for each row
extern uint8 flagDetectLeft[imgRow];
extern uint8 flagDetectRight[imgRow];
extern float detectDistance;
#define laneFound 1
#define laneMiss 0

#define laneNear    15
//#define laneFar       10

// roundabout status flag
#define enterRoundabout 1
#define exitRoundabout  0
extern int16 flagEnterRoundabout;
extern uint8 enterRoundaboutTimer;

#define rangeCountJitter    20

#define laneGainNear 1
#define laneGainFar 1

extern int16 jitterGlobal;
#define jitterGlobalNear 40
#define jitterGlobalFar 10

#define laneFilterStart 5

/* not activated
ushort jitterBendRight = 0;
ushort jitterBendLeft = 0;
*/

// predefined lane half width
extern int32 laneWidth[imgRow];

// sharp curve
extern uint8 sharpCurveRow;
#define rangeSharpCurveRow      35

// working temp
extern uint8 iterRow, iterCol;
extern uint8 iterRowSub, iterColSub;

// parameter

/* not activated
#define rangeLittleSBand    40
*/

extern int32 slope;
extern uint8 slopeRowStart, slopeRowEnd;
extern uint8 specialCaseStart;

#define rangeComputeSlopeFar    30
#define rangeComputeSlopeNear   49

extern int32 cameraError;
extern int32 cameraErrorPrevious;


// working temp in regression
extern uint16 numRegRow;
extern uint16 sumLaneCenter;
extern int32 sumNum, sumDen;
extern int32 averLaneCenter, averRowIndex;

void laneInit();

extern int32 laneJitterLeft;
extern int32 laneJitterRight;
extern uint8 countJitterBreakRowLeft;
extern uint8 countJitterBreakRowRight;

extern int32 pixelMeanPrevious;
extern int32 pixelMeanThres;
extern int32 pixelMean;

extern int32 *referArray;
extern int32 *destiArray;
extern uint8 *flagDetectRefer;
extern uint8 *flagDetectDesti;

#define rowStartThreeWayDetect  15
#define rowStopThreeWayDetect   13
#define colStartThreeWayDetect  91
#define colStopThreeWayDetect   97
#define threeWayDetectThreshold 100 * (colStopThreeWayDetect - colStartThreeWayDetect) * (rowStopThreeWayDetect - rowStartThreeWayDetect)
#define threeWayLaneThres       100

extern uint16 detectPointSum;
extern uint8 flagEnterThreeWay;
extern uint8 flagDetectedThreeWayFeature;

//extern uint16 areaDetectRoundabout;
extern uint16 areaDetectRoundaboutLeft;
extern uint16 areaDetectRoundaboutRight;

extern uint8 missCounterLeft;
extern uint8 missCounterRight;
extern uint8 missCounterBoth;

extern int32 sharpCurveStatus;

extern int16 sharpCurveJitterThres;

extern uint8 detectUpperMissingLeft;
extern uint8 detectLowerMissingLeft;
extern uint8 missingLaneUpperLeft;
extern uint8 missingLaneLowerLeft;

extern uint8 detectUpperMissingRight;
extern uint8 detectLowerMissingRight;
extern uint8 missingLaneUpperRight;
extern uint8 missingLaneLowerRight;

extern float slopeDetectRoundabout;
extern float roundaboutSlopeRowLocation;
extern uint8 roundaboutDetectionStartRow;
extern uint16 curveError;
extern int32 bigCurveThres;

extern int32 laneLocationShifted;
extern int32 laneLocationShiftedPrevious;

extern uint8 roundaboutPhase;
extern int32 rowBreachUpper;
extern int32 rowBreachLower;
extern int32 breachDetectLocation;
extern int32 enterBreachSlope;

// used in detect roundabout
extern int32 laneLocationShiftedLower;
extern int32 laneLocationShiftedUpper;

extern uint16 startlineJumpingPointNum;
extern uint16 startlineJumpingPointNumThres;
extern uint8 flagEnterStartLine;

extern int32 outboundAreaBenchmark;
extern int32 outboundAreaSum;
extern int32 outboundAreaThres;
extern uint8 flagEnterOutbound;
extern uint16 exitOutboundDelay;

#endif
