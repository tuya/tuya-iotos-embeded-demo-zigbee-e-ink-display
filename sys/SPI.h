#ifndef __SPI_H
#define __SPI_H 		
#include "MY_ST_config.h"

unsigned char r_dat_bat(unsigned long address,unsigned long DataLen,unsigned char *pBuff);
unsigned char r_dat(uint32_t address);
unsigned char CheckID(unsigned char CMD, unsigned long address,unsigned long byte_long,unsigned char *p_arr);

void zk_init(void);
void GT5S_DeepSleep(void);
void GT5S_Wakeup(void);
void SEAT_SET(void);
void SEAT_CLOSE(void);
void ST_ADD1(void);
void ST_ADD2(void);
void ST_ADD3(void);
void ST_DEL1(void);
void ST_DEL2(void);
void ST_DEL3(void);
void ST_ALL1(void);
void ST_ALL2(void);
void ST_ALL3(void);
void SEAT_SET(void);
#endif

