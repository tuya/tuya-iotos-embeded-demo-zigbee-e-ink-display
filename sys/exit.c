#include "exit.h"
#include "USART.h"
#include "MY_ST_config.h"
#include "delay.h"
#include "RCC.h"
#include "IO.h"
#include "mcu_api.h"
#include "protocol.h"
#include "system.h"
#include "zigbee.h"


void low_power(void)
{
   unsigned int time_stamp;
   unsigned int work_time;
   unsigned int wt_long = 80;
	if (work_time == wt_long)
	{
        mcu_dp_bool_update(DPID_LOW_POWER,0); 
	}
	else
		mcu_dp_bool_update(DPID_LOW_POWER,1); 
 	
}

void key_Init(void)
{
	NVIC_SetPriority(EXTI9_5_IRQn, 1);
	NVIC_EnableIRQ(EXTI9_5_IRQn);

}


void EXTI9_5_IRQHandler(void)
{
	if (KEY_STATE == 0)
	{
		delay_ms(10);    //����

		if (KEY_STATE == 0)
	{
	    mcu_network_start();     //��������
	}
		else {
		}
		//while(!KEY1_OFF);     //�ȴ������ɿ�
	}
	else {
	}

	EXTI->PR1 = 1<<6;    //���line6 �ϵ��жϱ�־λ
}

void EXTI4_IRQHandler(void)
{
	Exit_stop_mode();
	EXTI->PR1 = 1<<4;    //���line4 �ϵ��жϱ�־λ

}
