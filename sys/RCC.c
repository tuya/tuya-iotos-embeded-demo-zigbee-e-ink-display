#include "RCC.h"
#include "USART.h"
#include "IO.h"
#include "epaper.h"
void Enter_stop_mode(void)
{
	RCC->APB1ENR1|=1<<28;    //PWR��Դ�ӿ�ʱ��ʹ��
	RCC->APB1RSTR1|=1<<17;    //  ʧ�ܴ���2
	RCC->APB1RSTR1|=1<<18;    //  ʧ�ܴ���3
	RCC->AHB2ENR|=0<<1;       //  ��ֹGPIO B
    RCC->AHB2ENR|=0<<2;       //  ��ֹGPIO C
	RCC->APB1ENR1|= 0<<14;       //��ֹSpiʱ��
	
	SPI_MISO_SLEEP;
	EPD_BUSY_SLEEP;
	EPD_DeepSleep();
	
    //PA4��Ϊ�ⲿ�жϻ�������
    Exit_IO4();
	//����STOP2ģʽ
	__WFI();

}


void Exit_stop_mode(void)
{
	SystemClock_Config();
	IO_Init();
	RCC->AHB2ENR|=1<<1;      
    RCC->AHB2ENR|=1<<2; 
}


void Exit_IO4(void)
{
	PA4_IN;              //����ģʽ
	EXTI->IMR1|=1<<4;     //�ж�����δ����
	EXTI->RTSR1|=1<<4;    //line4  ������  	
	NVIC_SetPriority(EXTI4_IRQn, 1);
	NVIC_EnableIRQ(EXTI4_IRQn);	
	EXTI->PR1 = 1<<4;    //���line4�ϵ��жϱ�־λ
	
}

void SystemClock_Config(void)     //����HSI16��Ϊϵͳʱ��
{
	RCC->APB1ENR1 |= 1<<28;       //ʹ�ܵ�Դ�ӿ�ʱ��           																								
    PWR->CR1 = 1<<9;              //ѡ��1.8V��Χ		
  while((PWR->SR2 & 1<<10) != 0)  //�ȴ���ѹ�������ѹ����Ϊ���� VOS ��ƽ
	{
		
	}
	
  RCC->CR |= 1<<8 ;//ʹ��HSI   																								
  while ((RCC->CR & (1<<10)) != (1<<10)) //�ȴ�HSI�񵴾���
  {
		
  } 
	
	RCC->CFGR &= ~(3<<0);
  RCC->CFGR |= (1<<0); //����HSI16��Ϊϵͳʱ��
  while ((RCC->CFGR & 1<<2)  != 1<<2) //�ȴ�HSI16��Ϊϵͳʱ��
  {

  }
	
	RCC->CFGR |=1<<15;//ѡ���ڲ� 16 MHz (HSI16) ������Ϊ��ֹͣģʽ����ʱ��     STOPWUCKλ
    RCC->CSR |= 1<<0 ;//ʹ��	LSI   																								
  while ((RCC->CSR & (1<<1)) != (1<<1)) //�ȴ�LSI�񵴾���
  {
		
  }	
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;  
	PWR->CR1 |= 2<<0;
}




