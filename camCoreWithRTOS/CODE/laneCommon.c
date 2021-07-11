#include "headfile.h"
//#include "laneCommon.h"

// Mat outMat;  // this is used to store the results in thresholdAdapt
//

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
uint8 enterRoundaboutLeft = 0;
uint8 enterRoundaboutRight = 0;

/* not activated
ushort jitterBendRight = 0;
ushort jitterBendLeft = 0;
*/

// predefined lane half width
int32 laneWidth[imgRow] = {
    16, 17, 18, 19, 21, 22, 23, 25, 26, 27,
    29, 31, 32, 33, 35, 36, 37, 39, 40, 41,
    42, 44, 45, 46, 48, 49, 50, 52, 53, 55,
    56, 57, 59, 60, 61, 63, 65, 65, 67, 68,
    69, 71, 73, 73, 75, 77, 78, 79, 82, 83,
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
uint8 slopeRowStart=0, slopeRowEnd=0;
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

int16 flagEnterRoundabout = 0;

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

uint16 areaDetectRoundaboutLeft = 0;
uint16 areaDetectRoundaboutRight = 0;

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

float slopeDetectRoundabout = 0;
float roundaboutSlopeRowLocation = 0;

uint8 enterRoundaboutTimer;
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
uint16 startlineJumpingPointNumThres = 50;
uint8 flagEnterStartLine = 0;

uint16 outboundAreaBenchmark = 0;
uint16 outboundAreaSum = 0;
uint16 outboundAreaThres = 0;
uint8 flagEnterOutbound = 0;
uint16 exitOutboundDelay = 0;
