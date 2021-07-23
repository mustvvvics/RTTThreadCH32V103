#include "headfile.h"

/*
 *  FLASH_SECTION_00-FLASH_SECTION_15
 *  FLASH_PAGE_0-FLASH_PAGE_3
 *  len 1-256
 */
uint32 write_buff[10];//flash
uint32 read_buff[10] ;

uint8 flashCheck = 0;
uint8 flashClear = 1;
void mainFlashRead(void){
    flashCheck = flash_check(FLASH_SECTION_15,FLASH_PAGE_0);
    if (flashCheck == 1) {
        flash_page_read(FLASH_SECTION_15, FLASH_PAGE_0, read_buff,10);
        expected_y = read_buff[0];

        flashCheck = 0;
    }
    else {
        return;
    }

}

void mainFlashWrite(void){
    flashClear = flash_erase_page(FLASH_SECTION_15,FLASH_PAGE_0);
    if (flashClear == 0) {
        flash_page_program(FLASH_SECTION_15, FLASH_PAGE_0, write_buff, 10);
        write_buff[0] = expected_y;

        flashClear = 1;
    }
    else {
        return;
    }
}


