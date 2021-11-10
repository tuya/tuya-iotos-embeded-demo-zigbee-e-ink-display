
#ifndef __EXIT_H
#define __EXIT_H
#include "stm32l433xx.h"

//PB6    KEY1
#define KEY_IN  {RCC->AHB2ENR|=1<<1;GPIOB->MODER&=~(3<<12);GPIOB->MODER|=0<<12;GPIOB->PUPDR|=1<<12;} 
#define KEY_STATE ((GPIOB->IDR & 1<<6) == 1<<6)

#define KEY1_ON()  0
#define KEY1_OFF() 1
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void NVIC_Configuration(void);
#endif

