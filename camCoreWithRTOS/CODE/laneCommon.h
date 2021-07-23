#ifndef _laneCommon_h
#define _laneCommon_h

// **************** uncomment this ********************
#include "headfile.h"
// ****************************************************

// ***************** comment this *********************
// laneBinarize
//#define uint8 unsigned char
//#define uint16 unsigned short
//#define uint32 unsigned int
//#define uint64 unsigned long
//
//#define int8 char
//#define int32 int
//#define int16 short
// ****************************************************

#define imgRow 50
#define imgCol 188
// ***************** comment this *********************
//typedef uint8 Mat[imgRow][imgCol];
// ****************************************************

#define max(x,y) ((x) > (y) ? (x) : (y))
#define min(x,y) ((x) < (y) ? (x) : (y))
#define abs(x) ((x) > 0 ? (x) : -(x))
#define rangeBiNear 10

// laneAnalyze
extern int32 laneLocationLeft[imgRow];
extern int32 laneLocationRight[imgRow];

extern int32 laneCenter[imgRow];
extern int32 laneCenterPrevious;

// lane detection flag for each row
extern uint8 flagDetectLeft[imgRow];
extern uint8 flagDetectRight[imgRow];
extern float detectDistance;
#define laneFound 1
#define laneMiss 0

#define laneNear    20
//#define laneFar       10

// roundabout status flag
#define enterRoundabout 1
#define exitRoundabout  0
extern int8 flagEnterRoundabout;
extern int32 areaDetectRoundaboutLeft;
extern int32 areaDetectRoundaboutRight;
extern int32 areaDetectRoundaboutThresLeft;
extern int32 areaDetectRoundaboutThresRight;
extern uint16 exitRoundaboutDelay;
extern uint8 enterRoundaboutDelay;

#define rangeCountJitter    15

#define laneGainNear 1
#define laneGainFar 1

extern int16 jitterGlobal;
#define jitterGlobalNear 40
#define jitterGlobalFar 10

#define laneFilterStart 5

// predefined lane half width
extern int32 laneWidth[imgRow];

// sharp curve
extern uint8 sharpCurveRow;
extern uint8 rangeSharpCurveRow;

// working temp
extern uint8 iterRow, iterCol;
extern uint8 iterRowSub, iterColSub;

extern int32 slope;
extern uint8 slopeRowStart, slopeRowEnd;

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
extern uint8 exitOutboundDelay;
extern uint8 confirmOutboundDelay;
extern uint8 exitRoundaboutStraightLaneNum;
extern uint8 rangeDetectOutBound;

extern uint8 accelerateRatio;
extern uint8 flagCameraElement;

extern uint8 detectCrossroadStartRow;
extern uint8 detectCrossroadEndRow;
extern uint8 detectCrossroadMissingNumThres;
extern uint8 exitCrossroadDelay;
extern uint8 flagEnterCrossroad;

extern uint8 crossroadMissNumLeft;
extern uint8 crossroadMissNumRight;
extern uint16 crossroadWidthSum;

extern uint8 detectThreeWayRoadStartRow;
extern uint8 detectThreeWayRoadEndRow;
extern uint8 detectThreewayFeatureLeft;
extern uint8 detectThreewayFeatureRight;
extern uint16 exitThreewayDelay;
extern uint8 enterThreewayDelay;
extern uint8 detectThreewayFeatureNum;
extern uint8 detectThreewayFeatureMaxNum;
extern uint8 detectThreewayFeatureNumThres;
extern int8 globalCenterBias;
extern uint8 laneLocationThreewayRoad;
extern uint8 detectThreewayFeatureNearestRow;
extern int16 detectThreewayFeatureNearestRowLeft;
extern int16 detectThreewayFeatureNearestRowRight;
extern int16 detectThreewayFeatureNearestRowWidthThres;
extern int16 detectThreewayFeatureNearestRowWidth;
extern float detectThreewayFeatureNearestRowRatio;
extern uint8 detectLaneWidthStartRow;
extern uint8 detectLaneWidthEndRow;
extern uint8 threewayFeatureRow;
extern uint8 flagThreewayFeatureFound;
extern uint8 threewayFeatureJumpWhiteNum;
extern uint8 threewayFeatureJumpBlackNum;
extern int32 threewayFeatureStartCol;
extern int32 threewayFeatureEndCol;
extern uint8 threewayFeatureStep;
extern uint8 threewayFeatureJumpPointLeft;
extern uint8 threewayFeatureJumpPointRight;
extern uint8 threewayFeatureWidth;

// detectThreewayroad3
extern uint8 leftStartFlagThreewayFeatureFound;
extern uint8 rightStartFlagThreewayFeatureFound;
extern uint16 leftStartThreewayFeatureJumpPointLeft;
extern uint16 rightStartThreewayFeatureJumpPointRight;
extern uint16 rightStartThreewayFeatureJumpPointLeft;
extern uint16 leftStartThreewayFeatureJumpPointRight;
extern uint16 leftStartThreewayFeatureWidth;
extern uint16 rightStartThreewayFeatureWidth;
extern uint16 threewayFeatureWidthSum;

extern uint16 laneWidthPresent[imgRow];
extern int32 laneWidthLeft[imgRow];
extern int32 laneWidthRight[imgRow];
extern int32 latestWhiteRowPixelMean;
extern uint8 laneCenterBiased;
extern int32 pixelMeanNear;
extern uint16 laneWidthCenter[imgRow];
extern uint8 maxAvailableRow;
extern uint8 threewayWidthFeatureRow;
extern int32 threewayWidthFeatureRowCenterJitter;
extern int32 laneWidthSlopeLeft;
extern int32 laneWidthSlopeRight;
#endif
