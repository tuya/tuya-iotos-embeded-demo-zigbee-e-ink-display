#include "sys.h"
#include "USART.h"
#include "delay.h"
#include "io.h"
#include "mcu_api.h"
#include "protocol.h"
#include "system.h"
#include "zigbee.h"
#include "exit.h"
#include "RCC.h"
int  main(void)
{
	System_Init();
	zigbee_protocol_init();
	delay_ms(2000);
	while(1)
	{
		zigbee_uart_service();

	}
}