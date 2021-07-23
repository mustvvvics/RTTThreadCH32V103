#include "headfile.h"

/*
 *  FLASH_SECTION_00-FLASH_SECTION_15
 *  FLASH_PAGE_0-FLASH_PAGE_3
 *  len 1-256
 */
uint32 write_buff[10] = {0};//flash
uint32 read_buff[10] = {0};

uint8 flashCheck = 0;

void mainFlashRead(void){
    flashCheck = flash_check(FLASH_SECTION_15,FLASH_PAGE_0);
    if (flashCheck == 1) {
        flash_page_read(FLASH_SECTION_15, FLASH_PAGE_0, read_buff,10);
        expected_y = read_buff[0];
    }
    else {
        expected_y = 0;
    }

}

void mainFlashWrite(void){

//����1�����ݣ�����0û�����ݣ������Ҫ�������ݵ�����д���µ�������Ӧ�ö������������в�������

    flash_erase_page(FLASH_SECTION_15,FLASH_PAGE_0);


    write_buff[0] = expected_y;
    flash_page_program(FLASH_SECTION_15, FLASH_PAGE_0, write_buff, 10);

}


