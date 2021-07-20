#include "headfile.h"

uint32 write_buff[10];//flash
uint32 read_buff[10];

uint8 readVar = 0;
uint8 readMax = 32;
void mainFlashRead(){
//    flash_page_read(FLASH_SECTION_15, FLASH_PAGE_0, read_buff,8);
    for (readVar = 0; readVar < readMax; ++readVar) {

    }
}



uint8 writeVar = 0;
uint8 writeMax = 32;
void mainFlashWrite(){
//    flash_page_program(FLASH_SECTION_15, FLASH_PAGE_0, write_buff, 8);
    for (writeVar = 0; writeVar < writeMax; ++writeVar) {

    }
}
