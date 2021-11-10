#include "USART.h"
#include "mcu_api.h"
#include "zigbee.h"

uint8_t USART_Send(USART_TypeDef * MY_usart,uint8_t *data,uint16_t len)
{
	uint8_t ret = 1;
	uint16_t timeout = 0x8000;
	while (len>0)
	{
		timeout=0x8000;
		MY_usart->TDR = *data;
		while ((MY_usart->ISR&1<<6)!=1<<6)
		{
			timeout--;
			if (0 == timeout)
			{
				ret = 1;
				break;
			}
		}
		data++;
		len--;
	}
	if (0 != timeout)
	{
		ret = 0;
	}
	return ret;
}


void usart2_sendbyte(unsigned char byte)      //单字节发送函数
{

	USART2->TDR = byte; 
	while ((USART2->ISR&1<<6)!=1<<6);	  //等待发送完毕
}


void usart3_sendbyte(unsigned char byte)    //单字节发送函数
{

	USART2->TDR = byte; 
	while((USART2->ISR&1<<6)!=1<<6);	  //等待发送完毕
}


//�UUART2
void Configure_USART_ZIGBEE(uint32_t bound)    //PA2 MTX    PA3 MRX     USART2
{
	RCC->APB1RSTR1 &=~(1<<17);    //恢复串口2
	RCC->AHB2ENR |= 1<<0;         //使能GPIOA时钟
	GPIOA->MODER &=~(3<<4|3<<6);        
	GPIOA->MODER |=2<<4|2<<6;  
	GPIOA->AFR[0] &=~(0xf<<8|0xf<<12);
    GPIOA->AFR[0] |=7<<8|7<<12;
	RCC->APB1ENR1 |=1<<17;        //使能串口2时钟
	USART_ZIGBEE->BRR = 16000000 / bound; 
	USART_ZIGBEE->CR1 |= 1<<0|1<<2|1<<3|1<<5;   
	NVIC_SetPriority(USART2_IRQn, 1);
	NVIC_EnableIRQ(USART2_IRQn);	
}


void USART2_IRQHandler(void)
{
	uint8_t usart2_data_receive;
	if ((USART_ZIGBEE->ISR & 1<<5))     //接收数据寄存器不为空
	{
        usart2_data_receive = USART_ZIGBEE->RDR; 
		uart_receive_input((uint8_t)usart2_data_receive);
	}
	else
	{

	}
	if ((USART_ZIGBEE->ISR & (1<<3)) == (1<<3))//ORE
	{
		USART_ZIGBEE->ICR =1<<3;
	}
}


#if 1//USART_LOG
void Configure_USART_LOG(uint32_t bound) //PB10 TXD  PB11 RXD  USART3
{
	RCC->APB1RSTR1 &=~(1<<18);      //恢复串口3
	RCC->AHB2ENR |= 1<<1;          //使能GPIOB时钟
	GPIOB->MODER &=~(3<<20|3<<22);     //第10位 11位
	GPIOB->MODER |=2<<20|2<<22;        //
	GPIOB->AFR[1] &=~(0xf<<8|0xf<<12);
	GPIOB->AFR[1] |=7<<8|7<<12;
	RCC->APB1ENR1 |=1<<18;         //使能串口3时钟
	
	USART_LOG->BRR = 16000000 / bound; 
	USART_LOG->CR1 |= 1<<0|1<<2|1<<3|1<<5;     
	while((USART_LOG->ISR & 1<<6) != 1<<6)     //发送完成标志位
	{ 
		break;/* add time out here for a robust application */
	}	
	NVIC_SetPriority(USART3_IRQn, 1);
	NVIC_EnableIRQ(USART3_IRQn);	
}


void USART3_IRQHandler(void)
{
	if((USART_LOG->ISR & 1<<5) == 1<<5)      //接收数据寄存器不为空
	{

	}
	else
	{

	}	
	if((USART_LOG->ISR & (1<<3)) == (1<<3))//ORE
	{
		USART_LOG->ICR =1<<3;
	}
}

#endif

#if 1

int _ttywrch(int ch)    
{
    ch=ch;
	return ch;
}

struct __FILE
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 

/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       

void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数
#define USART_fput USART_LOG
int fputc(int ch, FILE *f)
{      
	while((USART_fput->ISR&0X40)==0);     //循环发送，直到发送完毕
	USART_fput->TDR = (uint8_t) ch;          	
	return ch;
}
#endif
