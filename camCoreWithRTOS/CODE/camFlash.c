#include "headfile.h"

/*
 *  FLASH_SECTION_00-FLASH_SECTION_15
 *  FLASH_PAGE_0-FLASH_PAGE_3
 *  len 1-256
 */
uint16 buffMax = 14;
uint32 write_buff[14] = {0} ;//flash
uint32 read_buff[14] = {0};

uint8 flashCheck = 0;
void camFlashRead(void){
    flashCheck = flash_check(FLASH_SECTION_15,FLASH_PAGE_0);
    if (flashCheck == 1) {
        flash_page_read(FLASH_SECTION_15, FLASH_PAGE_0, read_buff,buffMax);
        drivingDirection = read_buff[0];
        elementTableLengthFromMain = read_buff[1];
        elementTableFromMain = read_buff[2];
        detectDistance = (read_buff[3]/10) + (read_buff[3] % 10) * 0.1; //(line[6] / 10) + ((line[6] % 10) * 0.1)
        pixelMeanThres = read_buff[4];
        slopeRowStart = read_buff[5];
        slopeRowEnd = read_buff[6];
        areaDetectRoundaboutThresLeft = read_buff[7];
        areaDetectRoundaboutThresRight = read_buff[8];
        roundaboutDetectionStartRow = read_buff[9];
        detectCrossroadMissingNumThres = read_buff[10];
        rangeSharpCurveRow = read_buff[11];
        globalCenterBias = read_buff[12];
        startlineJumpingPointNumThres = read_buff[13];
//read_buff[14];
//read_buff[15];
//read_buff[16];
//read_buff[17];
//read_buff[18];
    }
    else {
        elementTableLengthFromMain = 0;elementTableFromMain = 0;

        drivingDirection = 1;detectDistance = 1.7;pixelMeanThres = 100;
        slopeRowStart=48;slopeRowEnd=35;
        areaDetectRoundaboutThresLeft = 1000;areaDetectRoundaboutThresRight = 1000;
        roundaboutDetectionStartRow = 20;globalCenterBias = -7;startlineJumpingPointNumThres = 50;
    }

}
//uint8 flashClear = 1;
void camFlashWrite(void){

    flash_erase_page(FLASH_SECTION_15,FLASH_PAGE_0);
    write_buff[0] = drivingDirection;//方向
    write_buff[1] = elementTableLengthFromMain;//顺序表长度
    write_buff[2] = elementTableFromMain;//顺序表
    write_buff[3] = (int16)(detectDistance * 10);
    write_buff[4] = pixelMeanThres;
    write_buff[5] = slopeRowStart;
    write_buff[6] = slopeRowEnd;
    write_buff[7] = areaDetectRoundaboutThresLeft;
    write_buff[8] = areaDetectRoundaboutThresRight;
    write_buff[9] = roundaboutDetectionStartRow;
    write_buff[10] = detectCrossroadMissingNumThres;
    write_buff[11] = rangeSharpCurveRow;
    write_buff[12] = globalCenterBias;
    write_buff[13] = startlineJumpingPointNumThres;
    flash_page_program(FLASH_SECTION_15, FLASH_PAGE_0, write_buff, buffMax);

}


