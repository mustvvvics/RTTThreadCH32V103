// **************** comment this ****************
//#include "laneAnalyze.h"
//#include "laneCommon.h"
//#include <stdio.h>
// **********************************************

// *************** uncomment this ***************
#include "headfile.h"
// ***********************************************

void locateLaneByMeanSlide(Mat outMat) {
    //printf("\nlaneCenterPrevious: %d\n", laneCenterPrevious);
    pixelMeanPrevious = 2 * outMat[iterRow][laneCenterPrevious];
    //printf("pixelMeanPre: %d\n", pixelMeanPrevious);

    // left lane
    //printf("left start %d\n", iterRow);
    for (iterCol = laneCenterPrevious; iterCol > laneCenterPrevious - laneWidth[iterRow] / 2 * detectDistance && iterCol > 2; iterCol-=2) { // modified >1 to >2
        pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol-1];
        //printf("%3d\t", pixelMean);
        if (pixelMeanPrevious - pixelMean > pixelMeanThres) {
            laneLocationLeft[iterRow] = iterCol;
            flagDetectLeft[iterRow] = laneFound;
            break;
        } else {
            laneLocationLeft[iterRow] = 0;
            flagDetectLeft[iterRow] = laneMiss;
        }
    }

    // right lane
    //printf("\nright start %d\n", iterRow);
    for (iterCol = laneCenterPrevious; iterCol < laneCenterPrevious + laneWidth[iterRow] / 2 * detectDistance && iterCol < imgCol - 3; iterCol+=2) { // modified <imgCol-2 to <imgCol-3
        pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol+1];
        //printf("%3d\t", pixelMean);
        if (pixelMeanPrevious - pixelMean > pixelMeanThres && iterCol > laneLocationLeft[iterRow]) {
            laneLocationRight[iterRow] = iterCol;
            flagDetectRight[iterRow] = laneFound;
            break;
        }
        else {
            laneLocationRight[iterRow] = imgCol - 1;
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
    if (iterRow > slopeRowEnd) {
        flagDetectLeft[iterRow] = laneFound;
        flagDetectRight[iterRow] = laneFound;
        laneLocationLeft[iterRow] = 0;
        laneLocationRight[iterRow] = imgCol-1;
    }
    pixelMeanPrevious = 2 * outMat[iterRow][laneCenterPrevious];
    if (flagEnterRoundabout == -1) { // left roundabout mdoe
        //left lane
        for (iterCol = laneCenterPrevious; iterCol > 2; iterCol-=2) { // modified >1 to >2
            pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol-1];
            //printf("%3d\t", pixelMean);
            if (pixelMeanPrevious - pixelMean > pixelMeanThres) {
                laneLocationLeft[iterRow] = iterCol;
                flagDetectLeft[iterRow] = laneFound;
                break;
            } else {
                laneLocationLeft[iterRow] = laneLocationLeft[iterRow+1] + (imgRow-iterRow);
                flagDetectLeft[iterRow] = laneFound;
            }
        }

        // right lane
        //printf("\nright start %d\n", iterRow);
        for (iterCol = laneCenterPrevious; iterCol < imgCol*3/5; iterCol+=2) { // modified <imgCol-2 to <imgCol-3
            pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol+1];
            //printf("%3d\t", pixelMean);
            if (pixelMeanPrevious - pixelMean > pixelMeanThres && iterCol > laneLocationLeft[iterRow]) {
                laneLocationRight[iterRow] = iterCol;
                flagDetectRight[iterRow] = laneFound;
                break;
            }
            else {
                laneLocationRight[iterRow] = laneLocationLeft[iterRow] + laneWidth[iterRow];
                flagDetectRight[iterRow] = laneFound;
            }
        }
    } else { // right roundabout mode
        //left lane
        for (iterCol = laneCenterPrevious; iterCol > imgCol*2/5; iterCol-=2) { // modified >1 to >2
            pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol-1];
            //printf("%3d\t", pixelMean);
            if (pixelMeanPrevious - pixelMean > pixelMeanThres) {
                laneLocationLeft[iterRow] = iterCol;
                flagDetectLeft[iterRow] = laneFound;
                break;
            } else {
                laneLocationLeft[iterRow] = 0;
                flagDetectLeft[iterRow] = laneMiss;
            }
        }

        // right lane
        //printf("\nright start %d\n", iterRow);
        for (iterCol = laneCenterPrevious; iterCol < imgCol-2; iterCol+=2) { // modified <imgCol-2 to <imgCol-3
            pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol+1];
            //printf("%3d\t", pixelMean);
            if (pixelMeanPrevious - pixelMean > pixelMeanThres && iterCol > laneLocationLeft[iterRow]) {
                laneLocationRight[iterRow] = iterCol;
                flagDetectRight[iterRow] = laneFound;
                break;
            }
            else {
                laneLocationRight[iterRow] = imgCol - 1;
                flagDetectRight[iterRow] = laneMiss;
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
            laneLocationLeft[iterRow] = laneLocationLeft[iterRow+1];
            laneLocationRight[iterRow] = laneLocationRight[iterRow+1];
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
        if (laneCenter[iterRow] - laneCenter[iterRow + 1] > 4){
            laneCenter[iterRow] = laneCenter[iterRow + 1] + 1;
        }
        if (laneCenter[iterRow] - laneCenter[iterRow + 1] < -4){
            laneCenter[iterRow] = laneCenter[iterRow + 1] - 1;
        }
    }

    // restrict lane center location
    laneCenter[iterRow] = min(laneCenter[iterRow], imgCol-1);
    laneCenter[iterRow] = max(laneCenter[iterRow], 0);

    laneCenterPrevious = laneCenter[iterRow];
}

void keystoneCorrection() {
    for (iterRow = 0; iterRow < imgRow - 2; ++iterRow) {
        laneLocationLeft[iterRow] = ((laneLocationLeft[iterRow] - imgCol / 2) * laneWidth[49] / laneWidth[iterRow]) + imgCol / 2;
        laneLocationRight[iterRow] = ((laneLocationRight[iterRow] - imgCol / 2) * laneWidth[49] / laneWidth[iterRow]) + imgCol / 2;
        laneCenter[iterRow] = ((laneCenter[iterRow] - imgCol / 2) * laneWidth[49] / laneWidth[iterRow]) + imgCol / 2;
    }
}

void keystoneForCountJitter() {

}

void countJitter() {
    laneJitterLeft = laneJitterRight = 9999;
    countJitterBreakRowLeft = countJitterBreakRowRight = 0;

    if (flagDetectLeft[imgRow-2] && flagDetectLeft[imgRow-3]) {
        for (iterRow = imgRow-3; iterRow > rangeCountJitter; --iterRow) {
            countJitterBreakRowLeft = iterRow;
            if (flagDetectLeft[iterRow+1] && flagDetectLeft[iterRow] && \
                    !flagDetectLeft[iterRow-1] && !flagDetectLeft[iterRow-2]) {
                break;
            }
        }
        laneJitterLeft = 0;
        laneLocationShiftedPrevious = (laneLocationLeft[imgRow-2] - imgCol / 2) \
                                    * laneWidth[49] / laneWidth[iterRow];
        for (iterRow=imgRow-3; iterRow >= countJitterBreakRowLeft; --iterRow) {
            laneLocationShifted = (laneLocationLeft[iterRow] - imgCol / 2) \
                                    * laneWidth[49] / laneWidth[iterRow];
            laneJitterLeft += laneLocationShifted - laneLocationShiftedPrevious;
            laneLocationShiftedPrevious = laneLocationShifted;
        }
    }

    if (flagDetectRight[imgRow-2] && flagDetectRight[imgRow-3]) {
        for (iterRow = imgRow-3; iterRow > rangeCountJitter; --iterRow) {
            countJitterBreakRowRight = iterRow;
            if (flagDetectRight[iterRow+1] && flagDetectRight[iterRow] && \
                    !flagDetectRight[iterRow-1] && !flagDetectRight[iterRow-2]) {
                break;
            }
        }
        laneJitterRight = 0;
        laneLocationShiftedPrevious = (laneLocationRight[imgRow-2] - imgCol / 2) \
                                        * laneWidth[49] / laneWidth[iterRow];
        for (iterRow=imgRow-3; iterRow >= countJitterBreakRowRight; --iterRow) {
            laneLocationShifted = (laneLocationRight[iterRow] - imgCol / 2) \
                                        * laneWidth[49] / laneWidth[iterRow];
            laneJitterRight += laneLocationShiftedPrevious - laneLocationShifted;
            laneLocationShiftedPrevious = laneLocationShifted;
        }
    }
}

// sharpCurveStatus:
//  1   --  left big curve,
//  2   --  left sharp curve,
//  -1  --  right big curve,
//  -2  --  right sharp curve
void detectSharpCurve() {
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

    for (iterRow=imgRow-2; iterRow > sharpCurveRow; --iterRow) {
        //printf("flag: %d\n", flagDetectDesti[iterRow]);
        if (!flagDetectDesti[iterRow]) {
            //printf("adapted to %d\n", iterRow);
            destiArray[iterRow] = destiArray[iterRow+1] + \
                                    (referArray[iterRow+1] - referArray[iterRow]) * sharpCurveStatus;
        }
    }
    recomputeLaneCenter(imgRow-2, sharpCurveRow);
}

void detectSBend() {
    /* not activated
    // detect little s-bend
    if (iterRow > rangeLittleSBand) {
        if (laneCenter[iterRow] > laneCenter[iterRow + 2]){
            ++jitterBendRight;
        }
        if (laneCenter[iterRow] < laneCenter[iterRow + 2]){
            ++jitterBendLeft;
        }
    }
    */
}

// flagEnterRoundabout: 0 -- no roundabout, -1 -- left, 1 -- right
void detectRoundabout() {
    if (flagEnterRoundabout != 0) {
        if (exitRoundaboutDelay == 0) {
            exitRoundaboutDelay = 600;
        } else if (exitRoundaboutDelay == 100) {
            flagEnterRoundabout = 99;
            --exitRoundaboutDelay;
        } else if (exitRoundaboutDelay == 1) {
            flagEnterRoundabout = 0;
            areaDetectRoundaboutLeft = areaDetectRoundaboutRight = 0;
        } else {
            --exitRoundaboutDelay;
        }
        return;
    }


    detectUpperMissingLeft = detectLowerMissingLeft = 0;
    missingLaneUpperLeft = missingLaneLowerLeft = 0;

    detectUpperMissingRight = detectLowerMissingRight = 0;
    missingLaneUpperRight = missingLaneLowerRight = 0;

    areaDetectRoundaboutLeft = areaDetectRoundaboutRight = 0;
    slopeDetectRoundabout = roundaboutSlopeRowLocation = 0;

    // left lane
    if (flagDetectLeft[roundaboutDetectionStartRow]) {
        // upper
        for (iterRow = roundaboutDetectionStartRow; iterRow > 1; iterRow--) {
            if (flagDetectLeft[iterRow-1] || flagDetectLeft[iterRow-2]) {
                continue;
            }
            detectUpperMissingLeft = 1;
            missingLaneUpperLeft = iterRow;
            break;
        }

        // lower
        for (iterRow = roundaboutDetectionStartRow; iterRow < imgRow-2; iterRow++) {
            if (flagDetectLeft[iterRow+1] || flagDetectLeft[iterRow+2]) {
                continue;
            }
            detectLowerMissingLeft = 1;
            missingLaneLowerLeft = iterRow;
            break;
        }

        if (detectUpperMissingLeft && detectLowerMissingLeft) {
            laneLocationShiftedLower = (laneLocationLeft[missingLaneLowerLeft] -imgCol / 2) \
                                        * laneWidth[49] / laneWidth[missingLaneLowerLeft];
            laneLocationShiftedUpper = (laneLocationLeft[missingLaneUpperLeft] -imgCol / 2) \
                                        * laneWidth[49] / laneWidth[missingLaneUpperLeft];
            if (abs(laneLocationShiftedLower - laneLocationShiftedUpper) > 10) {
                return;
            }
            roundaboutSlopeRowLocation = min(laneLocationShiftedLower, laneLocationShiftedUpper);
            for (iterRow=missingLaneLowerLeft-1; iterRow>missingLaneUpperLeft+1; --iterRow) {
                areaDetectRoundaboutLeft += (laneLocationLeft[iterRow] - imgCol / 2) \
                                            * laneWidth[49] / laneWidth[iterRow]     \
                                            - roundaboutSlopeRowLocation;
            }
            if (areaDetectRoundaboutLeft > areaDetectRoundaboutThres) {
                laneCenterPrevious = imgCol / 4;
                flagEnterRoundabout = -1;
            }
            //printf("area roundabout left: %d\n", areaDetectRoundaboutLeft);
            //printf("roundabout start row %d, end row %d\n", missingLaneLowerLeft, missingLaneUpperLeft);
        }
    }

    // right lane
    if (flagDetectRight[roundaboutDetectionStartRow]) {
        // upper
        for (iterRow = roundaboutDetectionStartRow; iterRow > 1; iterRow--) {
            if (flagDetectRight[iterRow-1] || flagDetectRight[iterRow-2]) {
                continue;
            }
            detectUpperMissingRight = 1;
            missingLaneUpperRight = iterRow;
            break;
        }

        // lower
        for (iterRow = roundaboutDetectionStartRow; iterRow < imgRow-2; iterRow++) {
            if (flagDetectRight[iterRow+1] || flagDetectRight[iterRow+2]) {
                continue;
            }
            detectLowerMissingRight = 1;
            missingLaneLowerRight = iterRow;
            break;
        }

        if (detectUpperMissingRight && detectLowerMissingRight) {
            laneLocationShiftedLower = (laneLocationRight[missingLaneLowerRight] - imgCol / 2) \
                                        * laneWidth[49] / laneWidth[missingLaneLowerRight];
            laneLocationShiftedUpper = (laneLocationRight[missingLaneUpperRight] - imgCol / 2) \
                                        * laneWidth[49] / laneWidth[missingLaneUpperRight];
            if (abs(laneLocationShiftedLower - laneLocationShiftedUpper) > 10) {
                return;
            }
            roundaboutSlopeRowLocation = max(laneLocationShiftedLower, laneLocationShiftedUpper);
            for (iterRow=missingLaneLowerRight-1; iterRow>missingLaneUpperRight+1; --iterRow) {
                areaDetectRoundaboutRight += roundaboutSlopeRowLocation \
                                            - (laneLocationRight[iterRow] - imgCol / 2) \
                                            * laneWidth[49] / laneWidth[iterRow];
            }
            if (areaDetectRoundaboutRight > areaDetectRoundaboutThres) {
                laneCenterPrevious = imgCol / 4 * 3;
                flagEnterRoundabout = 1;
            }
            //printf("area roundabout right: %d\n", areaDetectRoundaboutRight);
            //printf("roundabout start row %d, end row %d\n", missingLaneLowerRight, missingLaneUpperRight);
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

// direction: -1 -- left, 1 -- right
void adaptRoundaboutLane(Mat outMat) {
    // if (flagEnterRoundabout == -1) {
    //  for (iterRow = slopeRowStart; iterRow > slopeRowEnd; --iterRow) {
    //      for (iterCol = imgCol/2; iterCol > 0; ){}
    //  }
    //  // for (iterRow = slopeRowStart; iterRow > slopeRowEnd; --iterRow) {
    //  //  outMat[iterRow][imgCol/2 + laneWidth[iterRow]/2] = 0;
    //  //  outMat[iterRow][imgCol/2 + laneWidth[iterRow]/2 + 1] = 0;
    // } else {
    //  //  outMat[iterRow][imgCol/2 - laneWidth[iterRow]/2] = 0;
    //  //  outMat[iterRow][imgCol/2 - laneWidth[iterRow]/2 - 1] = 0;
    //  // }
    // }

    // if (laneJitterLeft == 9999 && laneJitterLeft == 9999) {
    //  flagEnterRoundabout = 0;
    //  return;
    // }

    // if (flagEnterRoundabout == 1) {
    //  for (iterRow=imgRow-2; iterRow>0; --iterRow) {
    //      if (flagDetectLeft[iterRow]) {
    //          laneLocationRight[iterRow] = laneLocationLeft[iterRow] + laneWidth[iterRow];
    //      } else if (flagDetectRight[iterRow]) {
    //          if (laneLocationRight[iterRow]>laneLocationRight[iterRow+1]) {
    //              laneLocationRight[iterRow] = laneLocationRight[iterRow+1] - 5;
    //          }
    //          laneLocationLeft[iterRow] = laneLocationRight[iterRow] - laneWidth[iterRow];
    //      } else if (!flagDetectLeft[iterRow] && !flagDetectRight[iterRow]) {
    //          laneLocationLeft[iterRow] = laneLocationLeft[iterRow+1] - 5;
    //          laneLocationRight[iterRow] = laneLocationLeft[iterRow] + laneWidth[iterRow];
    //      }
    //  }
    //  recomputeLaneCenter(45, 0);
    // }

    // // phase 1: wait for a while
    // if (roundaboutPhase == 1) {
    //  if (enterRoundaboutTimer < 5) {
    //      ++enterRoundaboutTimer;
    //      return;
    //  }
    //
    //  // ready for phase 1
    //  roundaboutPhase = 2;
    //  enterRoundaboutTimer = 0;
    //  if (flagEnterRoundabout == 1) {
    //      destiArray = laneLocationLeft;  // find breach and interpolate here
    //      referArray = laneLocationRight;
    //      flagDetectDesti = flagDetectLeft;
    //      flagDetectRefer = flagDetectRight;
    //  } else {
    //      destiArray = laneLocationRight;
    //      referArray = laneLocationLeft;
    //      flagDetectDesti = flagDetectRight;
    //      flagDetectRefer = flagDetectLeft;
    //  }
    // }

    // // phase 2: detect breach at a point
    // if (roundaboutPhase == 2) {
    //  if (flagDetectDesti[breachDetectLocation]   || \
    //      flagDetectDesti[breachDetectLocation-1] || \
    //      flagDetectDesti[breachDetectLocation-2] || \
    //      flagDetectDesti[breachDetectLocation-3]) {
    //      return;
    //  }

    //  // find the breach error and keep the error for a little while
    //  if (enterRoundaboutTimer++ < 3) {
    //      iterRow = breachDetectLocation;
    //      // find breach upper
    //      for (iterRow=breachDetectLocation-1; iterRow > 12; --iterRow) {
    //          if (flagDetectDesti[iterRow-1]) {
    //              rowBreachUpper = iterRow;
    //              break;
    //          }
    //      }
    //      for (iterRow=breachDetectLocation; iterRow < 35; ++iterRow) {
    //          if (flagDetectDesti[iterRow+1]) {
    //              rowBreachLower = iterRow;
    //              break;
    //          }
    //      }

    //      enterBreachSlope = (referArray[rowBreachLower+1] - destiArray[rowBreachUpper-1]) / (rowBreachLower - rowBreachUpper) * -1;
    //      for (iterRow=rowBreachLower; iterRow>=rowBreachUpper; --iterRow) {
    //          destiArray[iterRow] = destiArray[iterRow+1] + enterBreachSlope;
    //          referArray[iterRow] = referArray[iterRow+1] + enterBreachSlope;
    //      }
    //      recomputeLaneCenter(rowBreachLower, rowBreachUpper);
    //      return;
    //  }

    //  // ready for phase 3
    //  roundaboutPhase = 3;
    //  enterRoundaboutTimer = 0;
    //  specialCaseStart = 5;
    // }

    // // phase 3: detect
    // if (roundaboutPhase == 3) {
    //  if (!sharpCurveStatus) {
    //      return;
    //  }

    //  roundaboutPhase = 0;
    //  flagEnterRoundabout = 0;
    // }
}

// flagEnterThreeWay: 0 -- not detected 1 -- prepare to enter
//               2 -- enterd       3 -- prepare to exit
void detectThreeWayRoad(Mat outMat) {
    for (iterRow=rowStartThreeWayDetect; \
            iterRow > rowStopThreeWayDetect; --iterRow) {
        for (iterCol=colStartThreeWayDetect; \
                iterCol < colStopThreeWayDetect; ++iterCol) {
            detectPointSum += outMat[iterRow][iterCol];
        }
    }

    if (detectPointSum > threeWayDetectThreshold) {
        for (iterRow=rowStartThreeWayDetect; \
                iterRow > rowStopThreeWayDetect; --iterRow) {
            pixelMeanPrevious = 2 * outMat[iterRow][imgCol/2];
            //printf("%dl:threeWayPixelMeanPre%d\n", iterRow, pixelMeanPrevious);
            for (iterCol = colStartThreeWayDetect; \
                    iterCol > colStartThreeWayDetect - 70; iterCol-=2) {
                pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol+1];
                //printf("l:pixelMean%d\n", pixelMean);
                if (pixelMean - pixelMeanPrevious > pixelMeanThres) {
                    //printf("got left\n");
                    flagDetectedThreeWayFeature = 1;
                    break;
                }
            }
            if (!flagDetectedThreeWayFeature) {
                return;
            }
            flagDetectedThreeWayFeature = 0;

            //printf("%dr:threeWayPixelMeanPre%d\n", iterRow, pixelMeanPrevious);
            for (iterCol = colStopThreeWayDetect; \
                    iterCol < colStopThreeWayDetect + 70; iterCol+=2) {
                pixelMean = outMat[iterRow][iterCol] + outMat[iterRow][iterCol+1];
                //printf("r:pixelMean%d\n", pixelMean);
                if (pixelMean - pixelMeanPrevious > pixelMeanThres) {
                    //printf("got right\n");
                    flagDetectedThreeWayFeature = 1;
                    break;
                }
            }
            if (!flagDetectedThreeWayFeature) {
                return;
            }
            flagDetectedThreeWayFeature = 0;
        }

        // confirmed to be a three-way road
        if (missCounterBoth > 25 && laneJitterLeft < 3 && laneJitterRight < 3) {
            flagEnterThreeWay = 1;
        }
//      switch (flagEnterThreeWay) {
//          case 0: flagEnterThreeWay = 1; break;
//          case 2: flagEnterThreeWay = 3; break;
//      }
//      return;
    }
//
//  // not a three-way road
//  switch (flagEnterThreeWay) {
//      case 1: flagEnterThreeWay = 2; break;
//      case 3: flagEnterThreeWay = 0; break;
//  }
}

void adaptThreeWayLane() {

}

void detectStartLine(Mat outMat) {
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
        flagEnterStartLine = 1;
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

            // recompare average pixel value
            outboundAreaSum = 0;
            for (iterRow = imgRow-1; iterRow > 45; --iterRow) {
                for (iterCol = 69; iterCol < imgCol-71; ++iterCol) {
                    outboundAreaSum += outMat[iterRow][iterCol];
                }
            }
            if (outboundAreaBenchmark - outboundAreaSum > outboundAreaThres) {
                flagEnterOutbound = 2; // out of bounds is confirmed
            } else {
                flagEnterOutbound = 0;
            }
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
    for (iterRow = imgRow-1; iterRow > 45; --iterRow) {
        for (iterCol = 69; iterCol < imgCol-71; ++iterCol) {
            outboundAreaSum += outMat[iterRow][iterCol];
        }
    }

    if (outboundAreaBenchmark - outboundAreaSum > outboundAreaThres) {
        flagEnterOutbound = 1; // suspect to be out of bounds
    }
}

void foresight() {

}

// 0 -- none, 1 -- threeWay,
// 2 -- startLine, 3 -- brake
void passParameter() {
    flagCameraElement = 0;
    if (flagEnterOutbound != 0) {
        flagCameraElement = 3;
    }
    if (flagEnterStartLine != 0) {
        flagCameraElement = 3;
    }
    if (flagEnterRoundabout==99) {
        flagCameraElement = 3;
    }
    if (flagEnterRoundabout!=0) {
        flagCameraElement = 3;
    }
}

uint8 detectCrossroadStartRow = 30;
uint8 detectCrossroadEndRow = 20;
uint8 detectCrossroadMissingNumThres = 6;
uint8 bothMissingNum = 0;
uint8 exitCrossroadDelay = 0;
uint8 flagEnterCrossroad = 0;

void detectCrossroad() {
    if (flagEnterCrossroad == 1) {
        if (exitCrossroadDelay == 0) {
            exitCrossroadDelay = 200;
        } else if (exitCrossroadDelay == 1) {
            exitCrossroadDelay = 0;
            flagEnterCrossroad = 0;
        }
        return;
    }

    bothMissingNum = 0;
    flagEnterCrossroad = 0;
    if (flagEnterRoundabout!=0) {
        return;
    }

    for (iterRow = detectCrossroadStartRow; iterRow > detectCrossroadEndRow; --iterRow) {
        if (!flagDetectLeft[iterRow] && !flagDetectRight[iterRow]) {
            ++bothMissingNum;
        }
    }
    if (bothMissingNum > detectCrossroadMissingNumThres) {
        flagEnterCrossroad = 1;
    }
}

void adaptCrossroad() {
    for (iterRow=slopeRowStart; iterRow > slopeRowEnd; --iterRow) {
        laneLocationLeft[iterRow] = max(laneLocationLeft[iterRow], imgCol/2-laneWidth[iterRow]);
        laneLocationRight[iterRow] = min(laneLocationRight[iterRow], imgCol/2+laneWidth[iterRow]);
    }
}

void laneAnalyze(Mat outMat){
    missCounterBoth = 0;
    missCounterLeft = 0;
    missCounterRight = 0;

    detectStartLine(outMat);
    detectOutOfBounds(outMat);

    // if (flagEnterRoundabout) {
    //  adaptRoundaboutLane(outMat);
    // }

    for (iterRow = imgRow-1; iterRow != 255; --iterRow){
        if (flagEnterRoundabout) {
            locateLaneByMeanSlide_and_adaptRoundaboutLane(outMat);
        } else {
            locateLaneByMeanSlide(outMat);
        }
        laneInterpolation();
        computeLaneCenter();
    }
    laneCenterPrevious = laneCenter[45];

    countJitter();

    // detectSBend();
    //detectThreeWayRoad(outMat);
    detectCrossroad();
    if (flagEnterCrossroad) {
        adaptCrossroad();
    }

    if (flagEnterRoundabout) {
        detectRoundabout();
    }
    detectSharpCurve();
    if (sharpCurveStatus) { //  && !flagEnterRoundabout
        adaptSharpCurve();
    }


    // add a strightline to help compute lane slope
    for (iterRow=imgRow-1; iterRow > slopeRowStart-1; --iterRow) {
        laneCenter[iterRow] = imgCol / 2 - 1;
    }

    passParameter();

    // for (iterRow = imgRow - 1; iterRow < 255; --iterRow) {
    //  //printf("rowCenter %d: %d\n", iterRow, laneCenter[iterRow]);
    // }
    // for (iterRow = imgRow - 1; iterRow < 255; --iterRow) {
    //  //printf("laneLocLeft %d: %d\n", iterRow, laneLocationLeft[iterRow]);
    // }
    // for (iterRow = imgRow - 1; iterRow < 255; --iterRow) {
    //  //printf("laneLocRight %d: %d\n", iterRow, laneLocationRight[iterRow]);
    // }

    //printf("flagEnterRoundabout: %d\n", flagEnterRoundabout);
    //printf("flagEnterThreeWay: %d\n", flagEnterThreeWay);
    //printf("laneJitterLeft: %d\n", laneJitterLeft);
    //printf("laneJitterRight: %d\n\n", laneJitterRight);
    //printf("sharpCurveStatus: %d\n", sharpCurveStatus);
    //printf("sharpCurveRow: %d\n", sharpCurveRow);
    //printf("roundaboutPhase: %d\n", roundaboutPhase);
    //printf("startlineJumpingPointNum: %d\n", startlineJumpingPointNum);
    //printf("outboundAreaBenchmark: %d\n", outboundAreaBenchmark);
    //printf("\n");

    // getLaneWidth();
}

void regression() {
    sumLaneCenter = 0;
    sumNum = 0;
    sumDen = 0;
    numRegRow = slopeRowStart - slopeRowEnd;
    //printf("numRegRow: %d\n", numRegRow);
    for (iterRow = slopeRowStart; iterRow > slopeRowEnd; --iterRow){
        sumLaneCenter += laneCenter[iterRow];
    }
    averLaneCenter = sumLaneCenter / numRegRow;
    averRowIndex = slopeRowEnd + (numRegRow / 2);

    // get slope with bisection method
    for (iterRow = slopeRowStart; iterRow > slopeRowEnd; --iterRow){
        sumNum += (laneCenter[iterRow] - averLaneCenter) * (iterRow - averRowIndex);
        sumDen += (iterRow - averRowIndex) * (iterRow - averRowIndex);
    }
    //printf("sumDen: %d\tsumNum: %d\n", sumDen, sumNum);
    slope = (sumDen == 0) ? 0 : (sumNum * 10 / sumDen);
}

// error composition: slope, front points jitter
void computeError() {
    // which method to limit slope?

    //slopeRowStart = rangeComputeSlopeNear + specialCaseStart;
    //slopeRowStart = 48;
    //slopeRowEnd = max(sharpCurveRow, rangeComputeSlopeFar);
    // if (laneJitterLeft > 10 && laneJitterRight > 10) {
    //  slopeRowEnd = 32;
    //} else {
    //  slopeRowEnd = 35;
    //}
    //printf("slope start at %d, end at %d\n", slopeRowStart, slopeRowEnd);
    regression();
    cameraError = slope;
    //cameraError = (cameraError * 5 + slope * 5) / 10; // moving average, loss pass filter
    // if (flagEnterRoundabout != 0) {
    //     cameraError = cameraError * 100 / 260;
    // }
    if (cameraError > 60) {
        cameraError = cameraErrorPrevious;
    } else {
        cameraErrorPrevious = cameraError;
    }
}
