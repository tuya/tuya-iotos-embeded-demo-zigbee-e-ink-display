#include "sys.h"
#include "RCC.h"
#include "delay.h"
#include "IO.h"
#include "USART.h"
#include "mcu_api.h"
#include "zigbee.h"
#include "SPI.h"
void System_Init(void)
{
	SystemClock_Config();	
	delay_init(16);
    IO_Init();
}

void System_Task(void)
{
	SPI_MISO_SLEEP;
	EPD_BUSY_SLEEP;
		__WFI();		
}

