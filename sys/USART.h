#ifndef __USART_H
#define __USART_H 		
#include "MY_ST_config.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

void Configure_USART_ZIGBEE(uint32_t bound); //PA2 MRX£¬PA3 MTX£¬USART2

void Configure_USART_LOG(uint32_t bound);     //TX PB11, RX PB10 USART3

uint8_t USART_Send(USART_TypeDef * MY_usart,uint8_t *data,uint16_t len);
void usart2_sendbyte(unsigned char byte);
void usart3_sendbyte(unsigned char byte);
#endif

