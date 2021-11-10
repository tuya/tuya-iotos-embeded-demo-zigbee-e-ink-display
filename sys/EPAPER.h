#ifndef __EPAPER_H
#define __EPAPER_H
#include "MY_ST_config.h"
#include "IO.h"
#include "delay.h"
//296*128///////////////////////////////////////

#define Full 1                  //全屏刷新
#define Partial  2              //局部刷新

#define POS 3
#define NEG 4 
#define OFF 5 


#define MAX_LINE_BYTES 50// =400/8
#define MAX_COLUMN_BYTES  300

#define ALLSCREEN_GRAGHBYTES	15000
#define PartialSCREEN_GRAGHBYTES	4680
///////////////////////////GPIO Settings//////////////////////////////////////////////////////

#define EPD_W21_MOSI_0	SPI_MOSI_RESET
#define EPD_W21_MOSI_1	SPI_MOSI_SET

#define EPD_W21_CLK_0	SPI_SCK_RESET
#define EPD_W21_CLK_1	SPI_SCK_SET

#define EPD_W21_CS_0	EPD_CS_RESET
#define EPD_W21_CS_1	EPD_CS_SET

#define EPD_W21_DC_0	EPD_DC_RESET
#define EPD_W21_DC_1	EPD_DC_SET

#define EPD_W21_RST_0	EPD_RST_RESET
#define EPD_W21_RST_1	EPD_RST_SET

#define EPD_W21_BUSY_LEVEL 0
#define isEPD_W21_BUSY EPD_BUSY


#define  driver_delay_xms(x) 	delay_ms(x)
//void EpaperIO_Init(void);
uint8_t Epaper_READBUSY(void);
void Epaper_Spi_WriteByte(unsigned char TxData);
void Epaper_Write_Command(unsigned char cmd);
void Epaper_Write_Data(unsigned char data);

void EPD_HW_Init(const unsigned char refresh_mode); //Electronic paper initialization
void EPD_W21_Init(void); //Electronic paper hard reset

void EPD_Part_Update(void); 
void EPD_Update(void);
	
void EPD_WhiteScreen_White(void);
void EPD_DeepSleep(void);
void EPD_WhiteScreen_Partial(void);

//Display 
void EPD_WhiteScreen_ALL(const unsigned char *datas);
void EPD_SetRAMValue_BaseMap(const unsigned char * datas);
void EPD_Dis_Part(unsigned int xstart,unsigned int ystart,const unsigned char * datas,unsigned int PART_LINE,unsigned int PART_COLUMN,unsigned char mode);
void EPD_Dis_Part2(unsigned int xstart,unsigned int ystart, unsigned char * datas,unsigned int PART_LINE,unsigned int PART_COLUMN);
void EPD_White_Part(unsigned int xstart,unsigned int ystart,const unsigned char * datas,unsigned int PART_LINE,unsigned int PART_COLUMN,unsigned char mode);

#endif


