#ifndef __IO_H
#define __IO_H 		
#include "math.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

//LED	 	PB1
#define LED_OUT {RCC->AHB2ENR|=1<<1;GPIOB->MODER&=~(3<<2);GPIOB->MODER|=1<<2;} 
#define LED_SET GPIOB->ODR|=1<<1
#define LED_RESET  GPIOB->ODR&=~(1<<1)
#define LED_TOG GPIOB->ODR^=1<<1
#define LED_ON() LED_SET
#define LED_OFF() LED_RESET

//LED1	 	PB2
#define LED1_OUT {RCC->AHB2ENR|=1<<1;GPIOB->MODER&=~(3<<4);GPIOB->MODER|=1<<4;} 
#define LED1_SET GPIOB->ODR|=1<<2
#define LED1_RESET  GPIOB->ODR&=~(1<<2)
#define LED1_TOG GPIOB->ODR^=1<<2
#define LED1_ON() LED1_SET
#define LED1_OFF() LED1_RESET

//LED2	 	PA11
#define LED2_OUT {RCC->AHB2ENR|=1<<0;GPIOA->MODER&=~(3<<22);GPIOA->MODER|=1<<22;} 
#define LED2_SET GPIOA->ODR|=1<<11
#define LED2_RESET  GPIOA->ODR&=~(1<<11)
#define LED2_TOG GPIOA->ODR^=1<<11
#define LED2_ON() LED2_SET
#define LED2_OFF() LED2_RESET


//PA5
#define PA5_OUT {RCC->AHB2ENR|=1<<0;GPIOA->MODER&=~(3<<10);GPIOA->MODER|=1<<10;} 
#define PA5_SET GPIOA->ODR|=1<<5
#define PA5_RESET  GPIOA->ODR&=~(1<<5)

//PA4
#define PA4_IN {RCC->AHB2ENR|=1<<0;GPIOA->MODER&=~(3<<8);GPIOA->MODER|=0<<8;} 
#define PA4_STATE ((GPIOA->IDR & 1<<4) == 1<<4)

//PC13
#define WKUP_IN  {RCC->AHB2ENR|=1<<2;GPIOC->MODER&=~(3<<26);GPIOC->MODER|=1<<26;}   //PC13»½ÐÑÒý½Å
#define WKUP_STATE ((GPIOC->IDR & 1<<13) == 1<<13)
//PB0
#define IO0_OUT {RCC->AHB2ENR|=1<<1;GPIOB->MODER&=~(3<<0);GPIOB->MODER|=0<<0;} 
#define IO0_SET GPIOB->ODR|=1<<0
#define IO0_RESET  GPIOB->ODR&=~(1<<0)

//PB4
#define PB4_OUT {RCC->AHB2ENR|=1<<1;GPIOB->MODER&=~(3<<8);GPIOB->MODER|=1<<8;}
#define PB4_SET GPIOB->ODR|=1<<4
#define PB4_RESET  GPIOB->ODR&=~(1<<4)
#define PB4_LOW() PB4_RESET

//PB5
#define PB5_OUT {RCC->AHB2ENR|=1<<1;GPIOB->MODER&=~(3<<10);GPIOB->MODER|=1<<10;}
#define PB5_SET GPIOB->ODR|=1<<5
#define PB5_RESET  GPIOB->ODR&=~(1<<5)
#define PB5_LOW() PB5_RESET


//PB13
#define SPI_SCK_OUT {RCC->AHB2ENR|=1<<1;GPIOB->MODER&=~(3<<26);GPIOB->MODER|=1<<26;}
#define SPI_SCK_SET GPIOB->ODR|=1<<13
#define SPI_SCK_RESET  GPIOB->ODR&=~(1<<13)

//PB14
#define SPI_MISO_IN {RCC->AHB2ENR|=1<<1;GPIOB->MODER&=~(3<<28);GPIOB->MODER|=0<<28;}
#define SPI_MISO_SET GPIOB->ODR|=1<<14
#define SPI_MISO_RESET  GPIOB->ODR&=~(1<<14)
#define SPI_MISO_STATE ((GPIOB->IDR & 1<<14) == 1<<14)
#define SPI_MISO_SLEEP {RCC->AHB2ENR|=1<<1;GPIOB->MODER&=~(3<<28);GPIOB->MODER|=3<<28;}

//PB15
#define SPI_MOSI_OUT {RCC->AHB2ENR|=1<<1;GPIOB->MODER&=~((uint32_t)3<<30);GPIOB->MODER|=(uint32_t)1<<30;}
#define SPI_MOSI_SET GPIOB->ODR|=1<<15
#define SPI_MOSI_RESET  GPIOB->ODR&=~(1<<15)

//PA12
#define LIB_CS_OUT {RCC->AHB2ENR|=1<<0;GPIOA->MODER&=~(3<<24);GPIOA->MODER|=1<<24;}
#define LIB_CS_SET GPIOA->ODR|=1<<12
#define LIB_CS_RESET  GPIOA->ODR&=~(1<<12)

//PB1
#define EPD_CS_OUT {RCC->AHB2ENR|=1<<1;GPIOB->MODER&=~(3<<2);GPIOB->MODER|=1<<2;}
#define EPD_CS_SET GPIOB->ODR|=1<<1
#define EPD_CS_RESET  GPIOB->ODR&=~(1<<1)

//PB5
#define EPD_DC_OUT {RCC->AHB2ENR|=1<<1;GPIOB->MODER&=~(3<<10);GPIOB->MODER|=1<<10;}
#define EPD_DC_SET GPIOB->ODR|=1<<5
#define EPD_DC_RESET  GPIOB->ODR&=~(1<<5)

//PB8
#define EPD_RST_OUT {RCC->AHB2ENR|=1<<1;GPIOB->MODER&=~(3<<16);GPIOB->MODER|=1<<16;}
#define EPD_RST_SET GPIOB->ODR|=1<<8
#define EPD_RST_RESET  GPIOB->ODR&=~(1<<8)

//PB9
#define EPD_BUSY_IN  {RCC->AHB2ENR|=1<<1;GPIOB->MODER&=~(3<<18);GPIOB->MODER|=0<<18;} 
#define EPD_BUSY ((GPIOB->IDR & 1<<9) == 1<<9)
#define EPD_BUSY_SLEEP  {RCC->AHB2ENR|=1<<1;GPIOB->MODER&=~(3<<18);GPIOB->MODER|=3<<18;} 

typedef struct {
  uint8_t seat_set[255];
  uint8_t st_qrcode[255];
  uint8_t st_qrcode1[255];
  uint8_t st_qrcode2[255];
  uint8_t st_add[255];
  uint8_t et_add[255];
  uint8_t n_add[255];
  uint8_t st_all[255];
  uint8_t et_all[255];
  uint8_t n_all[255];
  uint8_t low_power;
} TYPE_BUFFER_S;

void IO_Init(void);
void EPD(void);
void EPD_Partial(void);
void qrcod_test(void);
void key_Init(void);

#endif

