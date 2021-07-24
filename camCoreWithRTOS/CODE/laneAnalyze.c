// **************** comment this ****************
//#include "laneAnalyze.h"
//#include "laneCommon.h"
//#include <stdio.h>
//uint8 gyroRoundFinishFlag = 0;
//uint8 steerStatusFromMain = 0;
//uint8 carStart = 0;
//char *elementQueue = "***";
// **********************************************

// *************** uncomment this ***************
 #include "headfile.h"
// ***********************************************


int32 regression(uint8 slopeRowStart, uint8 slopeRowEnd, int32 *colArray);

void locateLaneByMeanSlide(Mat outMat) {
    //printf("\nlaneCenterPrevious: %d\n", laneCenterPrevious);
    pixelMeanPrevious = 2 * outMat[iterRow][laneCenterPrevious];
    //printf("pixelMeanPre: %d\n", pixelMeanPrevious);

    // left lane
    //printf("left start %d\n", iterRow);
    for (iterCol = laneCenterPrevious - 5; iterCol > laneCenterPrevious - laneWidth[iterRow] / 2 * detectDistance && iterCol > 2; iterCol-=2) {
        pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol-1];
        //printf("%3d\t", pixelMean);
        if (pixelMeanPrevious - pixelMean > pixelMeanThres) {
            laneLocationLeft[iterRow] = iterCol;
            flagDetectLeft[iterRow] = laneFound;
            break;
        } else {
            laneLocationLeft[iterRow] = globalCenterBias;
            flagDetectLeft[iterRow] = laneMiss;
        }
    }

    // right lane
    //printf("\nright start %d\n", iterRow);
    for (iterCol = laneCenterPrevious + 5; iterCol < laneCenterPrevious + laneWidth[iterRow] / 2 * detectDistance && iterCol < imgCol - 3; iterCol+=2) {
        pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol+1];
        //printf("%3d\t", pixelMean);
        if (pixelMeanPrevious - pixelMean > pixelMeanThres && iterCol > laneLocationLeft[iterRow]) {
            laneLocationRight[iterRow] = iterCol;
            flagDetectRight[iterRow] = laneFound;
            break;
        }
        else {
            laneLocationRight[iterRow] = imgCol + globalCenterBias;
            flagDetectRight[iterRow] = laneMiss;
        }
    }

    if (!flagDetectLeft[iterRow] && flagDetectRight[iterRow]) {
        missCounterLeft += 1;
    }
    if (!flagDetectRight[iterRow] && flagDetectLeft[iterRow]) {
        missCounterRight += 1;
    }
    if (!flagDetectLeft[iterRow] && !flagDetectRight[iterRow]) {
        missCounterBoth += 1;
    }
    //printf("c: %d\n", laneCenter[iterRow]);
}

void locateLaneByMeanSlide_and_adaptRoundaboutLane(Mat outMat) {
    if (iterRow == imgRow-1) {
        return;
    }

    if (iterRow < slopeRowEnd - 1) {
        flagDetectLeft[iterRow] = laneFound;
        flagDetectRight[iterRow] = laneFound;
        laneLocationLeft[iterRow] = globalCenterBias;
        laneLocationRight[iterRow] = imgCol+globalCenterBias;
        return;
    }
    pixelMeanPrevious = 2 * outMat[iterRow][laneCenterPrevious];
    if (flagEnterRoundabout == -2) { // left roundabout mdoe
        //left lane
        for (iterCol = laneCenterPrevious; iterCol > 2; iterCol-=2) { // modified >1 to >2
            pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol-1];
            //printf("%3d\t", pixelMean);
            if (pixelMeanPrevious - pixelMean > pixelMeanThres) {
                laneLocationLeft[iterRow] = iterCol;
                flagDetectLeft[iterRow] = laneFound;
                break;
            } else {
                laneLocationLeft[iterRow] = laneLocationLeft[iterRow+1] - (imgRow-iterRow);
                flagDetectLeft[iterRow] = laneFound;
            }
        }

        // right lane
        //printf("\nright start %d\n", iterRow);
        for (iterCol = laneCenterPrevious; iterCol < imgCol*3/5; iterCol+=2) {
            pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol+1];
            //printf("%3d\t", pixelMean);
            if (pixelMeanPrevious - pixelMean > pixelMeanThres && iterCol > laneLocationLeft[iterRow]) {
                laneLocationRight[iterRow] = iterCol;
                flagDetectRight[iterRow] = laneFound;
                break;
            } else {
                laneLocationRight[iterRow] = laneLocationLeft[iterRow] + laneWidth[iterRow] * 0.8;
                flagDetectRight[iterRow] = laneFound;
            }
        }
    } else if (flagEnterRoundabout == 2) { // right roundabout mode
        // right lane
        for (iterCol = laneCenterPrevious; iterCol < imgCol-3; iterCol+=2) {
            pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol+1];
            //printf("%3d\t", pixelMean);
            if (pixelMeanPrevious - pixelMean > pixelMeanThres) {
                laneLocationRight[iterRow] = iterCol;
                flagDetectRight[iterRow] = laneFound;
                break;
            } else {
                laneLocationRight[iterRow] = laneLocationRight[iterRow+1] + (imgRow-iterRow);
                flagDetectRight[iterRow] = laneFound;
            }
        }

        //left lane
        for (iterCol = laneCenterPrevious; iterCol > imgCol*2/5; iterCol-=2) {
            pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol-1];
            //printf("%3d\t", pixelMean);
            if (pixelMeanPrevious - pixelMean > pixelMeanThres && iterCol < laneLocationRight[iterRow]) {
                laneLocationLeft[iterRow] = iterCol;
                flagDetectLeft[iterRow] = laneFound;
                break;
            } else {
                laneLocationLeft[iterRow] = laneLocationRight[iterRow] - laneWidth[iterRow] * 0.8;
                flagDetectLeft[iterRow] = laneFound;
            }
        }
    } else {
        // left lane
        for (iterCol = laneCenterBiased; iterCol > laneCenterBiased - laneWidth[iterRow]/2 * 1.2; iterCol-=2) {
            pixelMean = outMat[iterRow][iterCol] + outMat[iterCol][iterCol-1];
            if (pixelMeanPrevious - pixelMean > pixelMeanThres) {
                laneLocationLeft[iterRow] = iterCol;
                flagDetectLeft[iterRow] = laneFound;
            } else {
                laneLocationLeft[iterRow] = imgCol/2 - laneWidth[iterRow]/2 + globalCenterBias;
                flagDetectLeft[iterRow] = laneFound;
            }
        }

        // right lane
        for (iterCol = laneCenterBiased; iterCol < imgCol/2-1 + laneWidth[iterRow]/2 * 1.2; iterCol+=2) {
            pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol+1];
            if (pixelMeanPrevious - pixelMean > pixelMeanThres) {
                laneLocationRight[iterRow] = iterCol;
                flagDetectRight[iterRow] = laneFound;
                break;
            } else {
                laneLocationRight[iterRow] = imgCol/2 + laneWidth[iterRow]/2 + globalCenterBias;
                flagDetectRight[iterRow] = laneFound;
            }
        }
    }
}

// lane interpolation with predifined lane width and previous lane bias
void laneInterpolation(){
    // do not do interpolation to the first line
    if (iterRow == imgRow - 1) {
        return;
    }

    // near-end lane interpolation, interpolate with predefined lane width
    if (iterRow > laneNear){
        if (flagDetectLeft[iterRow] && !flagDetectRight[iterRow]) {
            laneLocationRight[iterRow] = laneLocationLeft[iterRow] +\
                                        (laneGainNear  * laneWidth[iterRow]);
        }else if (!flagDetectLeft[iterRow] && flagDetectRight[iterRow]) {
            laneLocationLeft[iterRow] = laneLocationRight[iterRow] -\
                                        (laneGainNear  * laneWidth[iterRow]);
        }else if (!flagDetectLeft[iterRow] && !flagDetectRight[iterRow]) {
            laneLocationLeft[iterRow] = globalCenterBias;
            laneLocationRight[iterRow] = imgCol + globalCenterBias;
        }
    }
    // far-end lane interpolation, interpolate with previous lane bias
    else {
        if (flagDetectLeft[iterRow] && !flagDetectRight[iterRow]) {
            laneLocationRight[iterRow] = laneLocationRight[iterRow + 1] +\
                                        (laneGainFar * (laneLocationLeft[iterRow] - laneLocationLeft[iterRow + 1]));
        }else if (!flagDetectLeft[iterRow] && flagDetectRight[iterRow]) {
            laneLocationLeft[iterRow] = laneLocationLeft[iterRow + 1] +\
                                        (laneGainFar * (laneLocationRight[iterRow] - laneLocationRight[iterRow + 1]));
        }else if (!flagDetectLeft[iterRow] && !flagDetectRight[iterRow]) {
            laneLocationLeft[iterRow] = laneLocationLeft[iterRow+1];
            laneLocationRight[iterRow] = laneLocationRight[iterRow+1];
        }
    }
}

void computeLaneCenter() {
    // compute current lane center
    laneCenter[iterRow] = (laneLocationLeft[iterRow] + laneLocationRight[iterRow]) / 2;

    // lane center filter
    if (iterRow < imgRow - laneFilterStart){
        if (laneCenter[iterRow] - laneCenter[iterRow + 1] > 20) {
            laneCenter[iterRow] = laneCenter[iterRow + 1];
        } else if (laneCenter[iterRow] - laneCenter[iterRow + 1] > 7){
            laneCenter[iterRow] = laneCenter[iterRow + 1] + 3;
        }
        if (laneCenter[iterRow] - laneCenter[iterRow + 1] < -20) {
            laneCenter[iterRow] = laneCenter[iterRow + 1];
        } else if (laneCenter[iterRow] - laneCenter[iterRow + 1] < -7){
            laneCenter[iterRow] = laneCenter[iterRow + 1] - 3;
        }
    }

    // restrict lane center location
    laneCenter[iterRow] = min(laneCenter[iterRow], laneLocationRight[iterRow]-1);
    laneCenter[iterRow] = max(laneCenter[iterRow], laneLocationLeft[iterRow]+1);

    laneCenterPrevious = laneCenter[iterRow];
}

void countJitter() {
    laneJitterLeft = laneJitterRight = 9999;
    countJitterBreakRowLeft = countJitterBreakRowRight = 50;

    if (flagDetectLeft[imgRow-4] && flagDetectLeft[imgRow-5]) {
        for (iterRow = imgRow-5; iterRow > rangeCountJitter; --iterRow) {
            countJitterBreakRowLeft = iterRow;
            if (flagDetectLeft[iterRow+1] && flagDetectLeft[iterRow] && \
                    !flagDetectLeft[iterRow-1] && !flagDetectLeft[iterRow-2]) {
                break;
            }
        }
        laneJitterLeft = 0;
        laneLocationShiftedPrevious = (laneLocationLeft[imgRow-4] - imgCol / 2 + globalCenterBias) \
                                    * laneWidth[imgRow-4] / laneWidth[iterRow];
        for (iterRow=imgRow-5; iterRow >= countJitterBreakRowLeft; --iterRow) {
            laneLocationShifted = (laneLocationLeft[iterRow] - imgCol / 2 + globalCenterBias) \
                                    * laneWidth[imgRow-4] / laneWidth[iterRow];
            laneJitterLeft += laneLocationShifted - laneLocationShiftedPrevious;
            laneLocationShiftedPrevious = laneLocationShifted;
        }
    }

    if (flagDetectRight[imgRow-4] && flagDetectRight[imgRow-5]) {
        for (iterRow = imgRow-5; iterRow > rangeCountJitter; --iterRow) {
            countJitterBreakRowRight = iterRow;
            if (flagDetectRight[iterRow+1] && flagDetectRight[iterRow] && \
                    !flagDetectRight[iterRow-1] && !flagDetectRight[iterRow-2]) {
                break;
            }
        }
        laneJitterRight = 0;
        laneLocationShiftedPrevious = (laneLocationRight[imgRow-4] - imgCol / 2 + globalCenterBias) \
                                        * laneWidth[imgRow-4] / laneWidth[iterRow];
        for (iterRow=imgRow-5; iterRow >= countJitterBreakRowRight; --iterRow) {
            laneLocationShifted = (laneLocationRight[iterRow] - imgCol / 2 + globalCenterBias) \
                                        * laneWidth[imgRow-4] / laneWidth[iterRow];
            laneJitterRight += laneLocationShiftedPrevious - laneLocationShifted;
            laneLocationShiftedPrevious = laneLocationShifted;
        }
    }
}

void detectSharpCurve() {
    // sharpCurveStatus:
    //  1   --  left big curve,
    //  2   --  left sharp curve,
    //  -1  --  right big curve,
    //  -2  --  right sharp curve
    // detect sharp curve
    sharpCurveStatus = 0;
    sharpCurveRow = 0;
    curveError = 0;

    //printf("%d, %d\n", countJitterBreakRowLeft, countJitterBreakRowRight);
    if (laneJitterLeft == 9999 && laneJitterRight == 9999) {
        return;
    }

    if (laneJitterLeft > 80 && laneJitterRight == 9999) {
        sharpCurveStatus = -2;
        sharpCurveRow = countJitterBreakRowLeft;
        return;
    }

    if (laneJitterLeft == 9999 && laneJitterRight > 80) {
        sharpCurveStatus = 2;
        sharpCurveRow = countJitterBreakRowRight;
        return;
    }

    if (laneJitterRight < 0 && laneJitterLeft != 9999 && ((laneJitterLeft - laneJitterRight) > bigCurveThres)) {
        sharpCurveStatus = -1;
        sharpCurveRow = countJitterBreakRowLeft;
        curveError = countJitterBreakRowLeft - countJitterBreakRowRight;
        return;
    }

    if (laneJitterLeft < 0 && laneJitterRight != 9999 && ((laneJitterRight - laneJitterLeft) > bigCurveThres)) {
        sharpCurveStatus = 1;
        sharpCurveRow = countJitterBreakRowRight;
        curveError = countJitterBreakRowRight - countJitterBreakRowLeft;
        return;
    }
}

void recomputeLaneCenter(uint8 laneStartRecompute, uint8 laneStopRecompute) {
    for (iterRow=laneStartRecompute; iterRow > laneStopRecompute; --iterRow) {
        laneCenter[iterRow] = (laneLocationLeft[iterRow] + laneLocationRight[iterRow]) / 2;
    }
}

void adaptSharpCurve() {
    // referArray -- full lane
    // destiArray -- incomplete lane
    //printf("sharpCurveStatus: %d\n", sharpCurveStatus);
    if (sharpCurveStatus > 0) {
        referArray = laneLocationRight;
        destiArray = laneLocationLeft;
        flagDetectDesti = flagDetectLeft;
        sharpCurveStatus = -sharpCurveStatus;
    } else {
        referArray = laneLocationLeft;
        destiArray = laneLocationRight;
        flagDetectDesti = flagDetectRight;
    }

    for (iterRow=imgRow-5; iterRow > sharpCurveRow; --iterRow) {
        //printf("flag: %d\n", flagDetectDesti[iterRow]);
        if (!flagDetectDesti[iterRow]) {
            //printf("adapted to %d\n", iterRow);
            destiArray[iterRow] = destiArray[iterRow+1] + \
                                    (referArray[iterRow+1] - referArray[iterRow]) * sharpCurveStatus;
        }
    }
    recomputeLaneCenter(imgRow-5, sharpCurveRow);
}

void detectSBend() {
    // ^_^
    // :)
}

// flagEnterRoundabout: 0 -- no roundabout, -1 -- left, 1 -- right
void detectRoundabout(Mat outMat) {
    if (flagEnterRoundabout == 3) {
        if (exitRoundaboutDelay == 1) {
            flagEnterRoundabout = 0;
            ++iterElement;
            exitRoundaboutDelay = 0;
        } else {
            --exitRoundaboutDelay;
        }
        return;
    }

    if (enterRoundaboutDelay > 0) {
        --enterRoundaboutDelay;
        return;
    }

    if (abs(flagEnterRoundabout) == 2) {
        if (gyroRoundFinishFlag == 1) {
            flagEnterRoundabout = 3;
            exitRoundaboutDelay = 50;
            gyroRoundFinishFlag = 0;
        }
        return;
    }



    if (flagEnterRoundabout != 0) {
        if (flagEnterRoundabout == 1 || flagEnterRoundabout == -1) {
            switch (flagEnterRoundabout) {
                case 1:
                    if (countJitterBreakRowRight < 30) {
                        flagEnterRoundabout = 2;
                        enterRoundaboutDelay = 80;
                        gyroRoundFinishFlag = 0;
                        exitRoundaboutDelay = 0;
                    }
                    break;
                case -1:
                    if (countJitterBreakRowLeft < 30) {
                        flagEnterRoundabout = -2;
                        exitRoundaboutDelay = 0;
                        enterRoundaboutDelay = 80;
                        gyroRoundFinishFlag = 0;
                    }
                    break;
            }
            return;
        }
    }

    detectUpperMissingLeft = detectLowerMissingLeft = 0;
    missingLaneUpperLeft = missingLaneLowerLeft = 0;

    detectUpperMissingRight = detectLowerMissingRight = 0;
    missingLaneUpperRight = missingLaneLowerRight = 0;

    areaDetectRoundaboutLeft = areaDetectRoundaboutRight = 0;
    slopeDetectRoundabout = roundaboutSlopeRowLocation = 0;

    exitRoundaboutDelay = enterRoundaboutDelay = 0;


    // left lane
    if (flagDetectLeft[roundaboutDetectionStartRow]) {
        // upper
        for (iterRow = roundaboutDetectionStartRow; iterRow > 2; iterRow--) {
            if (flagDetectLeft[iterRow-1] || flagDetectLeft[iterRow-2]) {
                if (abs(laneLocationLeft[iterRow] - laneLocationLeft[iterRow+1]) < 10) {
                    continue;
                }
            }
            detectUpperMissingLeft = 1;
            missingLaneUpperLeft = iterRow;
            break;
        }

        // lower
        for (iterRow = roundaboutDetectionStartRow; iterRow < imgRow-4; iterRow++) {
            if (flagDetectLeft[iterRow+1] || flagDetectLeft[iterRow+2]) {
                if (abs(laneLocationLeft[iterRow] - laneLocationLeft[iterRow-1]) < 10) {
                    continue;
                }
            }
            detectLowerMissingLeft = 1;
            missingLaneLowerLeft = iterRow;
            break;
        }

        if (detectUpperMissingLeft && detectLowerMissingLeft) {
            laneLocationShiftedLower = (laneLocationLeft[missingLaneLowerLeft] - imgCol / 2 + globalCenterBias) \
                                        * laneWidth[49] / laneWidth[missingLaneLowerLeft];
            laneLocationShiftedUpper = (laneLocationLeft[missingLaneUpperLeft] - imgCol / 2 + globalCenterBias) \
                                        * laneWidth[49] / laneWidth[missingLaneUpperLeft];
            if (abs(laneLocationShiftedLower - laneLocationShiftedUpper) > 50) {
                return;
            }
            roundaboutSlopeRowLocation = min(laneLocationShiftedLower, laneLocationShiftedUpper);
            for (iterRow=missingLaneLowerLeft-1; iterRow>missingLaneUpperLeft+1; --iterRow) {
                areaDetectRoundaboutLeft += (laneLocationLeft[iterRow] - imgCol / 2 + globalCenterBias) \
                                            * laneWidth[49] / laneWidth[iterRow]     \
                                            - roundaboutSlopeRowLocation;
            }
            if (areaDetectRoundaboutLeft > areaDetectRoundaboutThresLeft && laneJitterRight < 40) {
                laneCenterPrevious = imgCol / 4 + globalCenterBias;
                flagEnterRoundabout = -1;
                return;
            }
        }
    }

    // right lane
    if (flagDetectRight[roundaboutDetectionStartRow]) {
        // upper
        for (iterRow = roundaboutDetectionStartRow; iterRow > 2; iterRow--) {
            if (flagDetectRight[iterRow-1] || flagDetectRight[iterRow-2]) {
                if (abs(laneLocationRight[iterRow] - laneLocationRight[iterRow+1]) < 10) {
                    continue;
                }
            }
            detectUpperMissingRight = 1;
            missingLaneUpperRight = iterRow;
            break;
        }

        // lower
        for (iterRow = roundaboutDetectionStartRow; iterRow < imgRow-4; iterRow++) {
            if (flagDetectRight[iterRow+1] || flagDetectRight[iterRow+2]) {
                if (abs(laneLocationRight[iterRow] - laneLocationRight[iterRow-1]) < 10) {
                    continue;
                }
            }
            detectLowerMissingRight = 1;
            missingLaneLowerRight = iterRow;
            break;
        }

        if (detectUpperMissingRight && detectLowerMissingRight) {
            laneLocationShiftedLower = (laneLocationRight[missingLaneLowerRight] - imgCol / 2 + globalCenterBias) \
                                        * laneWidth[49] / laneWidth[missingLaneLowerRight];
            laneLocationShiftedUpper = (laneLocationRight[missingLaneUpperRight] - imgCol / 2 + globalCenterBias) \
                                        * laneWidth[49] / laneWidth[missingLaneUpperRight];
            if (abs(laneLocationShiftedLower - laneLocationShiftedUpper) > 50) {
                return;
            }
            roundaboutSlopeRowLocation = max(laneLocationShiftedLower, laneLocationShiftedUpper);
            for (iterRow=missingLaneLowerRight-1; iterRow>missingLaneUpperRight+1; --iterRow) {
                areaDetectRoundaboutRight += roundaboutSlopeRowLocation \
                                            - (laneLocationRight[iterRow] - imgCol / 2 + globalCenterBias) \
                                            * laneWidth[49] / laneWidth[iterRow];
            }
            if (areaDetectRoundaboutRight > areaDetectRoundaboutThresRight && laneJitterLeft < 40) {
                laneCenterPrevious = imgCol * 3 / 4 + globalCenterBias;
                flagEnterRoundabout = 1;
                return;
            }
        }
    }
}

void getLaneWidth() {
    // get laneWidth
    for (iterRow = 0; iterRow < imgRow; ++iterRow) {
        if (iterRow % 10 == 0) {
            //printf("\n");
        }
        //printf("%d, ", laneLocationRight[iterRow] - laneLocationLeft[iterRow]);
    }
    //printf("\n");
}

void detectLaneWidth(Mat outMat) {
    pixelMeanNear = 2 * outMat[imgRow-1][laneCenterBiased];
    for (iterRow = imgRow-1; iterRow < 255; --iterRow) {
        pixelMeanPrevious = 2 * outMat[iterRow][laneCenterBiased];
        if (pixelMeanNear - pixelMeanPrevious > pixelMeanPrevious * 0.7) {
            laneWidthPresent[iterRow] = 9999;
            // //printf("row:%02d, laneWidth:%04d (predefined: %03d), laneCenter: %02d\n", iterRow, laneWidthPresent[iterRow], laneWidth[iterRow], laneWidthCenter[iterRow]);
            continue;
        }
        laneWidthLeft[iterRow] = laneWidthRight[iterRow] = 8888;
        for (iterCol = laneCenterBiased - 5; iterCol > 2; iterCol-=2) {
            pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol-1];
            if (pixelMeanPrevious - pixelMean > pixelMeanThres) {
                laneWidthLeft[iterRow] = iterCol;
                // //printf("laneWidthLeft in row %d is %d\n", iterRow, laneWidthLeft[iterRow]);
                break;
            }
        }
        for (iterCol = laneCenterBiased + 5; iterCol < imgCol-3; iterCol+=2) {
            pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol+1];
            if (pixelMeanPrevious - pixelMean > pixelMeanThres && iterCol > laneLocationLeft[iterRow]) {
                laneWidthRight[iterRow] = iterCol;
                // //printf("laneWidthRight in row %d is %d\n", iterRow, laneWidthRight[iterRow]);
                break;
            }
        }
        if (laneWidthLeft[iterRow] == 8888 || laneWidthRight[iterRow] == 8888) {
            laneWidthPresent[iterRow] = 8888;
        } else {
            laneWidthPresent[iterRow] = laneWidthRight[iterRow] - laneWidthLeft[iterRow];
            laneWidthCenter[iterRow] = (laneWidthRight[iterRow] + laneWidthLeft[iterRow]) / 2;
        }
        //printf("row:%02d, laneWidth:%04d (predefined: %03d), laneCenter: %02d\n", iterRow, laneWidthPresent[iterRow], laneWidth[iterRow], laneWidthCenter[iterRow]);
    }
}

void detectLaneWidthForThreeway() {
    if (flagEnterThreeWay == 2) {
        if (exitThreewayDelay != 0) {
            --exitThreewayDelay;
        } else {
            flagEnterThreeWay = 3;
            exitThreewayDelay = 80;
        }
        return;
    }

    if (flagEnterThreeWay == 3){
        if (exitThreewayDelay != 0) {
            --exitThreewayDelay;
        } else {
            flagEnterThreeWay = 0;
        }
        return;
    }

    if (flagEnterThreeWay == 1) {
        if (flagEnterOutbound) {
            flagEnterThreeWay = 2;
            exitThreewayDelay = 10;
        }
        return;
    }
    iterRow = imgRow;
    maxAvailableRow = 50;
    threewayWidthFeatureRow = 50;

    while (laneWidthPresent[--iterRow] < 7777) {
        maxAvailableRow = iterRow;
    }
    if (maxAvailableRow < 40) {
        laneWidthLeft[imgRow-1] = laneCenterBiased;
        laneWidthRight[imgRow-1] = laneCenterBiased;
        laneWidthSlopeLeft = regression(imgRow-1, maxAvailableRow, laneWidthLeft);
        laneWidthSlopeRight = regression(imgRow-1, maxAvailableRow, laneWidthRight);
    }

    if (laneWidthSlopeLeft > 8 && laneWidthSlopeRight < -8) {
        flagEnterThreeWay = 1;
    }

    //printf("threewayWidthFeatureRow: %d\n", threewayWidthFeatureRow);
    //printf("width slope left: %d\n", laneWidthSlopeLeft);
    //printf("width slope right: %d\n", laneWidthSlopeRight);
}

void detectTriangleForThreeway(Mat outMat) {
    if (flagEnterThreeWay == 2) {
        if (exitThreewayDelay != 0) {
            --exitThreewayDelay;
        } else {
            flagEnterThreeWay = 3;
            exitThreewayDelay = 200;
        }
        return;
    }

    if (flagEnterThreeWay == 3){
        if (exitThreewayDelay != 0) {
            --exitThreewayDelay;
        } else {
            flagEnterThreeWay = 0;
        }
        return;
    }

    if (flagEnterThreeWay == 1) {
        if (flagEnterOutbound) {
            flagEnterThreeWay = 2;
            exitThreewayDelay = 10;
        }
        return;
    }

    pixelMeanPrevious = 2 * outMat[imgRow-2][laneCenterBiased];
    pixelMean = 2 * outMat[threewayFeatureStartRow][laneCenterBiased];
    if (pixelMeanPrevious - pixelMean < pixelMeanThres) {
        //printf("nononono, white: %d, black: %d\n", pixelMeanPrevious, pixelMean);
        return;
    }

    pixelMeanPrevious = pixelMean;
    threewayFeatureCenterPrevious = laneCenterBiased;
    for (iterRow = threewayFeatureStartRow; iterRow < threewayFeatureStartRow + 20; ++iterRow) {
        threewayFeatureLeftFound = threewayFeatureRightFound = 0;
        threewayFeatureWidth[iterRow-threewayFeatureStartRow] = 255;
        threewayFeatureCenter[iterRow-threewayFeatureStartRow] = 255;
        //printf("\nrow %d, pixelMeanPrevious: %d\n", iterRow, pixelMeanPrevious);

        for (iterCol = threewayFeatureCenterPrevious - 1; iterCol > 3; --iterCol) {
            pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol-1];
            //printf("L%d:(%d)\t", iterCol, pixelMean);
            if (pixelMean - pixelMeanPrevious > pixelMeanThres) {
                //printf("\nfound a left: %d\n", iterCol);
                threewayFeatureLeft[iterRow-threewayFeatureStartRow] = iterCol;
                threewayFeatureLeftFound = 1;
                break;
            }
        }

        if (threewayFeatureLeftFound != 1) {
            //printf("\nnot found a left, row: %d\n", iterRow);
            break;
        }

        for (iterCol = threewayFeatureCenterPrevious + 1; iterCol < imgCol-4; ++iterCol) {
            pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol+1];
            //printf("R%d:(%d)\t", iterCol, pixelMean);
            if (pixelMean - pixelMeanPrevious > pixelMeanThres) {
                //printf("\nfound a right: %d\n", iterCol);
                threewayFeatureRight[iterRow-threewayFeatureStartRow] = iterCol;
                threewayFeatureRightFound = 1;
                break;
            }
        }

        if (threewayFeatureRightFound != 1) {
            //printf("not found a right, row: %d\n", iterRow);
            break;
        }
        threewayFeatureNearestRow = iterRow;
        threewayFeatureWidth[iterRow-threewayFeatureStartRow] = threewayFeatureRight[iterRow-threewayFeatureStartRow] - \
                                                                threewayFeatureLeft[iterRow-threewayFeatureStartRow];
        threewayFeatureCenterPrevious = (uint8)((uint16)threewayFeatureRight[iterRow-threewayFeatureStartRow] + \
                                        (uint16)threewayFeatureLeft[iterRow-threewayFeatureStartRow]) / 2;
        threewayFeatureCenter[iterRow-threewayFeatureStartRow] = threewayFeatureCenterPrevious;
        pixelMeanPrevious = 2 * outMat[iterRow][threewayFeatureCenterPrevious];
    }

    //printf("nearest row: %d\n", threewayFeatureNearestRow);
    for (iterRow = threewayFeatureStartRow; iterRow < threewayFeatureStartRow + 20; ++iterRow) {
        //printf("iterRow: %d, threewayFeatureWidth: %d, threewayFeatureCenter: %d\n", iterRow, threewayFeatureWidth[iterRow-threewayFeatureStartRow], threewayFeatureCenter[iterRow-threewayFeatureStartRow]);
    }

    if (threewayFeatureNearestRow < threewayFeatureStartRow + 6) {
        //printf("not enough!\n");
        return;
    }

    for (iterRow = threewayFeatureNearestRow-2; iterRow > threewayFeatureNearestRow - 5; --iterRow) {
        //printf("row: %d\n", iterRow);
        if (threewayFeatureWidth[iterRow-threewayFeatureStartRow] * laneWidth[49] / laneWidth[iterRow-threewayFeatureStartRow] < \
                threewayFeatureWidth[iterRow+1-threewayFeatureStartRow] * laneWidth[iterRow-threewayFeatureStartRow] / laneWidth[iterRow-threewayFeatureStartRow]) {
            //printf("shape error, row: %d, iterRow: %d, iterRow+1: %d\n", iterRow, threewayFeatureWidth[iterRow-threewayFeatureStartRow], threewayFeatureWidth[iterRow+1-threewayFeatureStartRow]);
            return;
        }
    }

    //printf("laneWidth: %d\n", laneWidthPresent[threewayFeatureNearestRow]);
    if (laneWidthPresent[threewayFeatureNearestRow] < laneWidth[iterRow] * 1.5) {
        //printf("white line not enough!\n");
        return;
    }

    if (laneWidthCenter[threewayFeatureNearestRow] != 0) {
        if (abs(laneWidthCenter[threewayFeatureNearestRow] - laneCenterBiased) > 20) {
            //printf("width not center, laneWidthCenter: %d\n", laneWidthCenter[threewayFeatureNearestRow]);
            return;
        }
    }

    for (iterRow = threewayFeatureNearestRow; iterRow < threewayFeatureNearestRow + 4; ++iterRow) {
        if (threewayFeatureNearestRow == 0) {
            continue;
        }

        if (abs(laneWidthCenter[iterRow] - laneCenterBiased) < 30) {
            //printf("you won threeway!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            flagEnterThreeWay = 1;
            return;
        }
    }
}

void detectStartLine(Mat outMat) {
    static uint8 exitStartLine = 0;

    if (exitStartLine > 0) {
        --exitStartLine;
        return;
    }

    startlineJumpingPointNum = 0;
    for (iterRow = 40; iterRow < imgRow; ++iterRow) {
        pixelMeanPrevious = outMat[iterRow][imgCol/3] * 2;
        for (iterCol = imgCol / 3; iterCol < imgCol * 2 / 3; iterCol += 2) {
            pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol+1];
            if (pixelMeanPrevious - pixelMean > pixelMeanThres || pixelMeanPrevious - pixelMean < -pixelMeanThres) {
                ++startlineJumpingPointNum;
            }
            pixelMeanPrevious = pixelMean;
        }
    }

    if (startlineJumpingPointNum > startlineJumpingPointNumThres) {
        if (startLineCounter == 0) {
            exitStartLine = 100;
            flagEnterStartLine = 1;
            ++iterElement;
        } else {
            exitStartLine = 100;
            flagEnterStartLine = 2;
        }
    } else {
        flagEnterStartLine = 0;
    }
}

void detectOutOfBounds(Mat outMat) {
    if (flagEnterOutbound == 1) {
        if (confirmOutboundDelay == 0) {
            confirmOutboundDelay = 200;
        } else if (confirmOutboundDelay == 1) {
            confirmOutboundDelay = 0;
            flagEnterOutbound = 0;
            // // recompare average pixel value
            // outboundAreaSum = 0;
            // for (iterRow = imgRow-1; iterRow > 45; --iterRow) {
            //     for (iterCol = 69; iterCol < imgCol-71; ++iterCol) {
            //         outboundAreaSum += outMat[iterRow][iterCol];
            //     }
            // }
            // if (outboundAreaBenchmark - outboundAreaSum > outboundAreaThres) {
            //     flagEnterOutbound = 2; // out of bounds is confirmed
            // } else {
            //     flagEnterOutbound = 0;
            // }
        } else {
            --confirmOutboundDelay;
        }
        return;
    }

    if (flagEnterOutbound == 2) {
        if (exitOutboundDelay == 0 && exitOutboundDelay != 1) {
            exitOutboundDelay = 200;
        } else if (exitOutboundDelay == 1) {
            exitOutboundDelay = 0;
            flagEnterOutbound = 0;
            ++iterElement;
        } else {
            --exitOutboundDelay;
        }
        return;
    }

    if (outboundAreaBenchmark == 0) {
        // init road info, get a sample of the front area as benchmark
        outboundAreaBenchmark = 0;
        for (iterRow = imgRow-1; iterRow > 45; --iterRow) {
            for (iterCol = 69; iterCol < imgCol-71; ++iterCol) {
                outboundAreaBenchmark += outMat[iterRow][iterCol];
            }
        }
        return;
    }

    outboundAreaSum = 0;
    if (flagEnterThreeWay == 1) {
        for (iterRow = imgRow-1; iterRow > rangeDetectOutBound; --iterRow) {
            for (iterCol = 69; iterCol < imgCol-71; ++iterCol) {
                outboundAreaSum += outMat[iterRow][iterCol];
            }
        }

        if (outboundAreaBenchmark - outboundAreaSum > outboundAreaThres) {
            flagEnterOutbound = 1; // suspect to be out of bounds
        }
    } else {
        for (iterRow = imgRow-1; iterRow > rangeDetectOutBound; --iterRow) {
            for (iterCol = 69; iterCol < imgCol-71; ++iterCol) {
                outboundAreaSum += outMat[iterRow][iterCol];
            }
        }

        if (outboundAreaBenchmark - outboundAreaSum > outboundAreaThres) {
            flagEnterOutbound = 1; // suspect to be out of bounds
        }
    }
}

void foresight() {
    // if (countJitterBreakRowLeft < rangeCountJitter + 3 && \
    //      countJitterBreakRowRight < rangeCountJitter + 3) {
    //  accelerateRatio = 12;
    // } else {
    //  accelerateRatio = 10;
    // }
    accelerateRatio = 10;
    if (flagEnterThreeWay == 1 || flagEnterThreeWay == 2 || flagEnterThreeWay == 3) {
        accelerateRatio = 7;
    }
}

void passParameter() {
    // 0 -- none, 1 -- threeWay,
    // 2 -- startLine, 3 -- brake
    // 4 -- roundabout

    flagCameraElement = 0;
    if (flagEnterStartLine != 0) {
        flagCameraElement = 3;
    }
    if (flagEnterRoundabout) {
        flagCameraElement = 4;
    }
    if (flagEnterThreeWay == 2) {
        flagCameraElement = 1;
    }
    if (flagEnterOutbound) {
        flagCameraElement = 3;
    }
//    if (flagEnterCrossroad) {
//        flagCameraElement = 3;
//    }
}

void detectCrossroad() {
    if (flagEnterCrossroad == 1) {
        if (exitCrossroadDelay == 0) {
            exitCrossroadDelay = 40;
        } else if (exitCrossroadDelay == 1) {
            ++iterElement;
            exitCrossroadDelay = 0;
            flagEnterCrossroad = 0;
        } else {
            --exitCrossroadDelay;
        }
        return;
    }

    crossroadMissNumLeft = 0;
    crossroadMissNumRight = 0;
    flagEnterCrossroad = 0;
    crossroadWidthSum = 0;

    for (iterRow = detectCrossroadStartRow+10; iterRow > detectCrossroadEndRow+10; --iterRow) {
        crossroadWidthSum += laneWidthPresent[iterRow];
    }

    if (crossroadWidthSum > 50000) {

        flagEnterCrossroad = 1;
    }

    // for (iterRow = detectCrossroadStartRow; iterRow > detectCrossroadEndRow; --iterRow) {
    //  if (!flagDetectLeft[iterRow]) {
    //      ++crossroadMissNumLeft;
    //  }
    //  if (!flagDetectRight[iterRow]) {
    //      ++crossroadMissNumRight;
    //  }
    // }
    // if (crossroadMissNumLeft > detectCrossroadMissingNumThres && crossroadMissNumRight > detectCrossroadMissingNumThres) {
    //  crossroadMissNumLeft = crossroadMissNumRight = 0;
    //  for (iterRow = slopeRowEnd-10; iterRow > slopeRowEnd-20; --iterRow) {
    //      if (!flagDetectLeft[iterRow]) {
    //          ++crossroadMissNumLeft;
    //      }
    //      if (!flagDetectRight[iterRow]) {
    //          ++crossroadMissNumRight;
    //      }
    //  }
    //  if (crossroadMissNumLeft < 10 - detectCrossroadMissingNumThres && crossroadMissNumRight < 10 - detectCrossroadMissingNumThres) {
    //      flagEnterCrossroad = 1;
    //  }
    // }
}

void markSlopeStartCenter() {
    // add a strightline to help compute lane slope
    for (iterRow=imgRow-1; iterRow > slopeRowStart-2; --iterRow) {
        laneCenter[iterRow] = imgCol / 2 + globalCenterBias;
    }
}

void delayForAWhile() {
    if (delayCounter == 0) {
        delayCounter = 40;
    } else if (delayCounter == 1) {
        delayCounter = 0;
        ++iterElement;
    } else {
        --delayCounter;
    }
}

void carRun() {
    iterElement = 0;
}

void laneAnalyze(Mat outMat){
    missCounterBoth = 0;
    missCounterLeft = 0;
    missCounterRight = 0;

    if (carStart == 1) {
        iterElement = 99;
        carStart = 0;
        startLineCounter = 0;
        flagEnterThreeWay = 0;
        flagEnterRoundabout = 0;
        flagEnterCrossroad = 0;
    }

    if (steerStatusFromMain == 0) {
        globalCenterBias = -7;
    } else if (steerStatusFromMain == 1){
        globalCenterBias = -13;
    } else if (steerStatusFromMain == 2) {
        globalCenterBias = -5;
    }

    detectOutOfBounds(outMat);

    // threeway mode does not need full camera error
    if (flagEnterThreeWay == 1 || flagEnterThreeWay == 2) {

         detectLaneWidthForThreeway();
//        detectTriangleForThreeway(outMat);
        flagEnterOutbound = 0;
        foresight();
        passParameter();
        return;
    }

    if (flagEnterThreeWay == 3) {
        flagEnterOutbound = 0;
    }

    for (iterRow = imgRow-4; iterRow != 255; --iterRow){
        if (abs(flagEnterRoundabout) == 2) {
            locateLaneByMeanSlide_and_adaptRoundaboutLane(outMat);
        } else {
            locateLaneByMeanSlide(outMat);
        }
        laneInterpolation();
        computeLaneCenter();
    }
    laneCenterPrevious = laneCenter[45];

    detectLaneWidth(outMat);
    countJitter();
    foresight();

    detectSharpCurve();
    if (sharpCurveStatus) { //  && !flagEnterRoundabout
        adaptSharpCurve();
    }

    if (iterElement == 99) {
        carRun();
        markSlopeStartCenter();
        return;
    }

    // detectTriangleForThreeway(outMat);
    // detectRoundabout(outMat);
    // detectCrossroad();
    if (elementQueue[iterElement] != '\0') {
        switch (elementQueue[iterElement]) {
            case '#':
                carRun();
                break;
            case '1':
                detectRoundabout(outMat);
                break;
            case '2':
                detectLaneWidthForThreeway();
                //detectTriangleForThreeway(outMat);
                break;
            case '3':
                detectCrossroad();
                break;
            case '5':
                detectStartLine(outMat);
                break;
            case '6':
                delayForAWhile();
                break;
            default:
                break;
        }
    }


    markSlopeStartCenter();
    passParameter();

    //printf("flagEnterRoundabout: %d\n", flagEnterRoundabout);
    //printf("flagEnterThreeWay: %d\n", flagEnterThreeWay);
    //printf("flagEnterThreeWay: %d\n", flagEnterThreeWay);
    //printf("laneJitterLeft: %d\n", laneJitterLeft);
    //printf("laneJitterRight: %d\n\n", laneJitterRight);
    //printf("sharpCurveStatus: %d\n", sharpCurveStatus);
    //printf("sharpCurveRow: %d\n", sharpCurveRow);
    //printf("roundaboutPhase: %d\n", roundaboutPhase);
    //printf("startlineJumpingPointNum: %d\n", startlineJumpingPointNum);
    //printf("outboundAreaBenchmark: %d\n", outboundAreaBenchmark);
    //printf("area roundabout left: %d\n", areaDetectRoundaboutLeft);
    //printf("roundabout start row %d, end row %d\n", missingLaneLowerLeft, missingLaneUpperLeft);
    //printf("area roundabout right: %d\n", areaDetectRoundaboutRight);
    //printf("roundabout start row %d, end row %d\n", missingLaneLowerRight, missingLaneUpperRight);
    //printf("\n");

    // getLaneWidth();
}

int32 regression(uint8 slopeRowStart, uint8 slopeRowEnd, int32 *colArray) {
    sumLaneCenter = 0;
    sumNum = 0;
    sumDen = 0;
    numRegRow = slopeRowStart - slopeRowEnd;
    //printf("numRegRow: %d\n", numRegRow);
    for (iterRow = slopeRowStart; iterRow > slopeRowEnd; --iterRow){
        sumLaneCenter += colArray[iterRow];
    }
    averLaneCenter = sumLaneCenter / numRegRow;
    averRowIndex = slopeRowEnd + (numRegRow / 2);

    // get slope with bisection method
    for (iterRow = slopeRowStart; iterRow > slopeRowEnd; --iterRow){
        sumNum += (colArray[iterRow] - averLaneCenter) * (iterRow - averRowIndex);
        sumDen += (iterRow - averRowIndex) * (iterRow - averRowIndex);
    }
    //printf("sumDen: %d\tsumNum: %d\n", sumDen, sumNum);
    return (sumDen == 0) ? 0 : (sumNum * 10 / sumDen);
}

// error composition: slope, front points jitter
void computeError() {
    if (flagEnterThreeWay==1 || flagEnterThreeWay == 2) {
        cameraError = 0;
        return;
    }

    //printf("slope start at %d, end at %d\n", slopeRowStart, slopeRowEnd);

    // error limit in detecting threeway
    if (flagEnterThreeWay) {
        slope = regression(slopeRowStart, slopeRowEnd+3, laneCenter);
    } else {
        slope = regression(slopeRowStart, slopeRowEnd, laneCenter);
    }

    // error limit in detecting roundabout
    if (abs(flagEnterRoundabout) == 1) {
        slope = min(slope, 3);
        slope = max(slope, -3);
    }
    cameraError = slope;

    // max error limit
    if (cameraError > 80 || cameraError < -80) {
        cameraError = cameraErrorPrevious;
    } else {
        cameraErrorPrevious = cameraError;
    }
}
