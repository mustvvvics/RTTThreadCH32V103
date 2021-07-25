#include "headfile.h"

/*
 *  FLASH_SECTION_00-FLASH_SECTION_15
 *  FLASH_PAGE_0-FLASH_PAGE_3
 *  len 1-256
 */
uint16 buffMax = 15;
uint32 write_buff[15] = {0};//flash
uint32 read_buff[15] = {0};

uint8 flashCheck = 0;

void mainFlashRead(void){
    flashCheck = flash_check(FLASH_SECTION_15,FLASH_PAGE_0);
    if (flashCheck == 1) {
        flash_page_read(FLASH_SECTION_15, FLASH_PAGE_0, read_buff,buffMax);
        drivingDirectionToCam = read_buff[0];
        elementTableLength = read_buff[1];
        elementTable = read_buff[2];
        fixCamDetectDistance = read_buff[3]; //注意在主核保存为整数

        fixCamPixelMeanThres = read_buff[4];
        fixCamSlopeRowStart = read_buff[5];
        fixCamSlopeRowEnd = read_buff[6];
        fixCamAreaDetectRoundaboutThresLeft = read_buff[7];
        fixCamAreaDetectRoundaboutThresRight = read_buff[8];
        fixCamRoundaboutDetectionStartRow = read_buff[9];
        fixCamDetectCrossroadMissingNumThres = read_buff[10];
        fixCamRangeSharpCurveRow = read_buff[11];
        fixCamGlobalCenterBias = read_buff[12];
        fixCamStartlineJumpingPointNumThres = read_buff[13];
        expected_y = read_buff[14];
    }
    else {
        expected_y = 50;
        elementTableLength = 9;elementTable = 113636225;
//        elementTableLength = 8;elementTable = 22363115;    //
//        drivingDirectionToCam = 1;fixCamDetectDistance = 17;fixCamPixelMeanThres = 100;
//        fixCamSlopeRowStart=48;fixCamSlopeRowEnd=35;
//        fixCamAreaDetectRoundaboutThresLeft = 370;fixCamAreaDetectRoundaboutThresRight = 370;
//        fixCamRoundaboutDetectionStartRow = 20;fixCamGlobalCenterBias = -7;
//        fixCamStartlineJumpingPointNumThres = 50;
    }

}
void sendFlashDataToCam(void){

    sendParameterToCam(8,0xDD,0,drivingDirectionToCam,0,0);
    sendParameterToCam(8,0xDC,0,elementTableLength,0,0);
    sendParameterToCam(32,0xDB,0,0,0,elementTable); //告知顺序
    sendParameterToCam(8,0xE8,0,fixCamDetectDistance,0,0);
    sendParameterToCam(32,0xE9,0,0,0,fixCamPixelMeanThres);
    sendParameterToCam(8,0xEA,0,fixCamSlopeRowStart,0,0);
    sendParameterToCam(8,0xEB,0,fixCamSlopeRowEnd,0,0);
    sendParameterToCam(32,0xEC,0,0,0,fixCamAreaDetectRoundaboutThresLeft);
    sendParameterToCam(32,0xED,0,0,0,fixCamAreaDetectRoundaboutThresRight);
    sendParameterToCam(8,0xAA,0,fixCamRoundaboutDetectionStartRow,0,0);
    sendParameterToCam(8,0xE4,0,fixCamDetectCrossroadMissingNumThres,0,0);
    sendParameterToCam(8,0xE5,0,fixCamRangeSharpCurveRow,0,0);
    sendParameterToCam(0,0xE6,fixCamGlobalCenterBias,0,0,0);
    sendParameterToCam(16,0xE7,0,0,fixCamStartlineJumpingPointNumThres,0);

    sendParameterToCam(8,0xDF,0,1,0,0);//让从机接受主机数据
}



void mainFlashWrite(void){

//返回1有数据，返回0没有数据，如果需要对有数据的区域写入新的数据则应该对所在扇区进行擦除操作

    flash_erase_page(FLASH_SECTION_15,FLASH_PAGE_0);
    write_buff[0] = drivingDirectionToCam;//方向
    write_buff[1] = elementTableLength;//顺序表长度
    write_buff[2] = elementTable;//顺序表
    write_buff[3] = fixCamDetectDistance;
    write_buff[4] = fixCamPixelMeanThres;
    write_buff[5] = fixCamSlopeRowStart;
    write_buff[6] = fixCamSlopeRowEnd;
    write_buff[7] = fixCamAreaDetectRoundaboutThresLeft;
    write_buff[8] = fixCamAreaDetectRoundaboutThresRight;
    write_buff[9] = fixCamRoundaboutDetectionStartRow;
    write_buff[10] = fixCamDetectCrossroadMissingNumThres;
    write_buff[11] = fixCamRangeSharpCurveRow;
    write_buff[12] = fixCamGlobalCenterBias;
    write_buff[13] = fixCamStartlineJumpingPointNumThres;

    write_buff[14] = expected_y;
    flash_page_program(FLASH_SECTION_15, FLASH_PAGE_0, write_buff, buffMax);

}


