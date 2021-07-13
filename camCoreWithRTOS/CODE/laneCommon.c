#include "headfile.h"
//#include "laneCommon.h"

// laneAnalyze
// lane location for each row
int32 laneLocationLeft[imgRow] = {0};
int32 laneLocationRight[imgRow] = {0};

int32 laneCenter[imgRow] = {imgCol / 2};
int32 laneCenterPrevious = imgCol / 2;

// lane detection flag for each row
uint8 flagDetectLeft[imgRow] = {0};
uint8 flagDetectRight[imgRow] = {0};
float detectDistance = 2.0;

// roundabout status flag
int8 flagEnterRoundabout = 0;
int32 areaDetectRoundaboutLeft = 0;
int32 areaDetectRoundaboutRight = 0;
int32 areaDetectRoundaboutThres = 300;

float slopeDetectRoundabout = 0;
float roundaboutSlopeRowLocation = 0;
uint8 roundaboutDetectionStartRow = 20;
uint8 exitRoundaboutDelay = 0;

/* not activated
ushort jitterBendRight = 0;
ushort jitterBendLeft = 0;
*/

// predefined lane half width
int32 laneWidth[imgRow] = {
    20, 26, 32, 32, 32, 32, 36, 36, 38, 40,
    42, 44, 46, 46, 48, 52, 52, 52, 52, 56,
    56, 56, 60, 62, 62, 64, 64, 68, 68, 68,
    72, 72, 74, 78, 78, 80, 82, 82, 84, 84,
    86, 88, 90, 92, 92, 94, 96, 96, 100, 100
};

// sharp curve
uint8 sharpCurveRow = 0;

// working temp
uint8 iterRow, iterCol = 0;
uint8 iterRowSub, iterColSub = 0;
uint8 location = 0;

// parameter

/* not activated
#define rangeLittleSBand    40
*/

int32 slope = 0;
uint8 slopeRowStart=48, slopeRowEnd=35;
uint8 specialCaseStart = 0;

int32 cameraError = 0;
int32 cameraErrorPrevious = 0;

int32 frontJitter = 0;

uint16 numRegRow;
uint16 sumLaneCenter;
int32 sumNum, sumDen;
int32 averLaneCenter, averRowIndex;

void laneInit() {
    for (iterRow = 0; iterRow < imgRow; ++iterRow) {
        laneLocationRight[iterRow] = imgCol - 1;
    }
}

int32 laneJitterLeft = 9999;
int32 laneJitterRight = 9999;
uint8 countJitterBreakRowLeft = 0;
uint8 countJitterBreakRowRight = 0;


int32 pixelMeanThres = 100;
int32 pixelMeanPrevious = 0;
int32 pixelMean = 0;

int32 *referArray;
int32 *destiArray;
uint8 *flagDetectRefer;
uint8 *flagDetectDesti;

uint16 detectPointSum = 0;
uint8 flagEnterThreeWay = 0;
uint8 flagDetectedThreeWayFeature = 0;

uint8 missCounterLeft = 0;
uint8 missCounterRight = 0;
uint8 missCounterBoth = 0;

int32 sharpCurveStatus = 0;

int16 sharpCurveJitterThres = 15;

uint8 detectUpperMissingLeft = 0;
uint8 detectLowerMissingLeft = 0;
uint8 missingLaneUpperLeft = 0;
uint8 missingLaneLowerLeft = 0;

uint8 detectUpperMissingRight = 0;
uint8 detectLowerMissingRight = 0;
uint8 missingLaneUpperRight = 0;
uint8 missingLaneLowerRight = 0;

uint16 curveError = 0;
int32 bigCurveThres = 100;

int32 laneLocationShifted = 0;
int32 laneLocationShiftedPrevious = 0;

uint8 roundaboutPhase = 0;
int32 rowBreachUpper = 0;
int32 rowBreachLower = 0;
int32 breachDetectLocation = 30;
int32 enterBreachSlope = 0;

// used in detect roundabout
int32 laneLocationShiftedLower = 0;
int32 laneLocationShiftedUpper = 0;

uint16 startlineJumpingPointNum = 0;
uint16 startlineJumpingPointNumThres = 40;
uint8 flagEnterStartLine = 0;

int32 outboundAreaBenchmark = 0;
int32 outboundAreaSum = 0;
int32 outboundAreaThres = 13000;
uint8 flagEnterOutbound = 0;
uint8 exitOutboundDelay = 0;
uint8 confirmOutboundDelay = 0;

uint8 accelerateRatio = 10;
uint8 flagCameraElement = 0;
