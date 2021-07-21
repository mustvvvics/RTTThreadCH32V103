#include "headfile.h"

/*
 *  FLASH_SECTION_00-FLASH_SECTION_15
 *  FLASH_PAGE_0-FLASH_PAGE_3
 *  len 1-256
 */
uint32 write_buff[10];//flash
uint32 read_buff[10] ;

uint8 readVar = 0;
uint8 readMax = 32;
uint8 flashCheck = 0;
void mainFlashRead(void){
    flashCheck = flash_check(FLASH_SECTION_15,FLASH_PAGE_0);
    if (flashCheck == 1) {
        flash_page_read(FLASH_SECTION_15, FLASH_PAGE_0, read_buff,10);
        expected_y = read_buff[0];
        //    for (readVar = 0; readVar < readMax; ++readVar) {
        //
        //    }
    }
    else {
        return;
    }

}



uint8 writeVar = 0;
uint8 writeMax = 32;
void mainFlashWrite(void){
    flash_page_program(FLASH_SECTION_15, FLASH_PAGE_0, write_buff, 10);
    write_buff[0] = expected_y;
//    for (writeVar = 0; writeVar < writeMax; ++writeVar) {
//
//    }
}


