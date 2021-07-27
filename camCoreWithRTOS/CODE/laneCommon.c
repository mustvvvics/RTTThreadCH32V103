#include "headfile.h"
//#include "laneCommon.h"

// laneAnalyze
// lane location for each row

uint8 encoderElementNum = 1;
int32 encoderNumEnterElement[20] = {120000};
int32 encoderNumExitElement[20] = {9999999};


int32 laneLocationLeft[imgRow] = {0};
int32 laneLocationRight[imgRow] = {0};

int32 laneCenter[imgRow] = {imgCol / 2};
int32 laneCenterPrevious = imgCol / 2;
uint8 laneCenterBiased = 0;

// lane detection flag for each row
uint8 flagDetectLeft[imgRow] = {0};
uint8 flagDetectRight[imgRow] = {0};
float detectDistance = 1.7;

// roundabout status flag
int8 flagEnterRoundabout = 0;
int32 areaDetectRoundaboutLeft = 0;
int32 areaDetectRoundaboutRight = 0;
int32 areaDetectRoundaboutThresLeft = 370;
int32 areaDetectRoundaboutThresRight = 370;

float slopeDetectRoundabout = 0;
float roundaboutSlopeRowLocation = 0;
uint8 roundaboutDetectionStartRow = 20;
uint16 exitRoundaboutDelay = 0;
uint8 enterRoundaboutDelay = 0;

/* not activated
ushort jitterBendRight = 0;
ushort jitterBendLeft = 0;
*/

// predefined lane half width
int32 laneWidth[imgRow] = {
    26, 28, 28, 30, 30, 32, 32, 34, 36, 36,
    38, 40, 40, 42, 44, 46, 46, 48, 50, 50,
    52, 54, 54, 56, 58, 60, 62, 64, 64, 68,
    68, 68, 70, 72, 74, 76, 76, 80, 80, 80,
    84, 86, 86, 88, 88, 90, 92, 94, 96, 98,
};

// int32 laneWidth[imgRow] = {
//  33, 35, 35, 37, 37, 39, 41, 41, 43, 43,
//  47, 47, 51, 51, 51, 55, 55, 55, 57, 59,
//  61, 63, 63, 63, 67, 67, 67, 71, 73, 73,
//  75, 77, 77, 79, 81, 83, 85, 85, 87, 87,
//  91, 91, 93, 95, 97, 99, 99, 101, 101, 103
// };

// sharp curve
uint8 sharpCurveRow = 0;
uint8 rangeSharpCurveRow = 35;

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

int32 cameraError = 0;
int32 cameraErrorPrevious = 0;

int32 frontJitter = 0;

uint16 numRegRow;
uint16 sumLaneCenter;
int32 sumNum, sumDen;
int32 averLaneCenter, averRowIndex;

void laneInit() {
    for (iterRow = 0; iterRow < imgRow; ++iterRow) {
        laneLocationRight[iterRow] = imgCol + globalCenterBias;
        laneWidthPresent[iterRow] = 9999;
    }
    laneCenterBiased = imgCol/2 + globalCenterBias;
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
uint16 startlineJumpingPointNumThres = 50;
int8 flagEnterStartLine = 0;

int32 outboundAreaBenchmark = 0;
int32 outboundAreaSum = 0;
int32 outboundAreaThres = 10000;
uint8 flagEnterOutbound = 0;
uint8 exitOutboundDelay = 0;
uint8 confirmOutboundDelay = 0;
uint8 exitRoundaboutStraightLaneNum = 0;
uint8 rangeDetectOutBound = 45;

uint8 accelerateRatio = 10;
uint8 flagCameraElement = 0;

uint8 detectCrossroadStartRow = 20;
uint8 detectCrossroadEndRow = 10;
uint8 detectCrossroadMissingNumThres = 6;
uint8 exitCrossroadDelay = 0;
uint8 flagEnterCrossroad = 0;
uint8 crossroadMissNumLeft = 0;
uint8 crossroadMissNumRight = 0;
uint16 crossroadWidthSum = 0;


int8 globalCenterBias = -7;

//detectCrossroad2
uint8 detectCrossroadRow[3] = {10, 25, 40};

// detectWidth
uint16 laneWidthCenter[imgRow] = {0};
int32 laneWidthLeft[imgRow] = {0};
int32 laneWidthRight[imgRow] = {0};
uint16 laneWidthPresent[imgRow] = {0};
// uint16 laneWidthLeft = 0;
// uint16 laneWidthRight = 0;
int32 latestWhiteRowPixelMean = 0;
int32 pixelMeanNear = 0;
uint8 maxAvailableRow = 0;
int32 laneWidthSlopeLeft = 0;
int32 laneWidthSlopeRight = 0;
uint8 iterElement = 0;
uint16 delayCounter = 0;

// threeway
uint8 exitThreewayDelay = 0;
uint8 flagEnterThreeWay = 0;
uint8 flagDetectedThreeWayFeature = 0;
uint8 threewayWidthFeatureRow = 0;
uint8 threewayFeatureStartRow = 5;
uint16 threewayFeatureLeft[30] = {0};
uint16 threewayFeatureRight[30] = {0};
uint16 threewayFeatureWidth[30] = {0};
uint16 threewayFeatureCenter[30] = {0};
uint16 threewayFeatureCenterPrevious = 0;
uint8 threewayFeatureLeftFound = 0;
uint8 threewayFeatureRightFound = 0;
uint8 threewayFeatureNearestRow = 0;
uint8 threewayDetectSequence = 0;

// start line
uint8 startLineTimes = 4;
uint8 exitStartlineCounter = 0;

// detectCrossroadScanLine
uint8 detectCrossroadStartRowScanLine = 25;
uint8 detectCrossroadLaneWidth = 0;
uint8 detectCrossroadLaneLeft = 0;
uint8 detectCrossroadLaneRight = 0;
uint8 detectCrossroadHeight[5] = {0};
uint8 detectCrossroadMaxHeight = 0;
uint8 detectCrossroadMaxHeightCol = 0;
float crossroadMaxAvilRow = 0;
float crossroadMaxAvilRowCol = 0;
float crossroadDesRow = 0;
float crossroadDesRowCol = 0;
float crossroadSlope = 0;
float currentCenter = 0;
