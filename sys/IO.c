#include "IO.h"
#include "delay.h"
#include "epaper.h"
#include "picture.h"
#include "SPI.h"
#include "USART.h"
#include "qrcode_create.h"
#include "tuya_qrcode_create.h"
#include "zigbee.h"

extern TYPE_BUFFER_S FlashBuffer;
extern unsigned char pBits[512];
void SPI_Init(void)
{
	SPI_SCK_OUT;
	SPI_MOSI_OUT;
	SPI_MISO_IN;
	LIB_CS_OUT;
	EPD_CS_OUT;
	EPD_DC_OUT;
	EPD_RST_OUT;
	EPD_BUSY_IN;
}

#define con(a,b) a##b
void EPD(void)
{
	EPD_HW_Init(Full); 											//Electronic paper initialization��ȫ��ˢ�³�ʼ����
	EPD_WhiteScreen_White();  									//Show all white
	EPD_DeepSleep(); 	
    EPD_HW_Init(Partial); 	
}

void EPD_Partial(void)
{
	EPD_HW_Init(Partial); 						                //Electronic paper initialization��ȫ��ˢ�³�ʼ����
	EPD_DeepSleep(); 	
	driver_delay_xms(1000);	
	}

	
TY_CREATE_IN_T Qrcod_CREATE_IN;
TY_CREATE_OUT_T *Qrcod_CREATE_OUT;
void qrcod_test(void)
{
	Qrcod_CREATE_IN.ecc_level=QRCODE_ECC_MEDIUM;
	Qrcod_CREATE_IN.version=9;
	Qrcod_CREATE_IN.magnifications=2;
	Qrcod_CREATE_IN.mode=STORAGE_MODE_BIT;
	Qrcod_CREATE_IN.information=FlashBuffer.st_qrcode;
	//Qrcod_CREATE_IN.information="www.tuya.com";
	tuya_svc_image_generate_qrcode_create(&Qrcod_CREATE_IN,&Qrcod_CREATE_OUT);
	EPD_HW_Init(Partial);
	EPD_Dis_Part2(288,150,Qrcod_CREATE_OUT->dst_data,112,106); 
	EPD_Part_Update();
	EPD_DeepSleep();
	tuya_svc_image_generate_qrcode_create_free(Qrcod_CREATE_OUT);
}


void led_Init(void)
{
	LED_OUT;
	LED_ON();
	LED1_OUT;
	LED1_OFF();
	LED2_OUT;
	LED2_OFF();	
}

void Modules_Init(void)
{
	SPI_Init();
    zk_init();
	EPD();
    EPD_Partial();
}


void IO_Init(void)
{
	led_Init();
	Configure_USART_LOG(USART_LOG_BOUND);              //USRT3
	Configure_USART_ZIGBEE(USART_ZIGBEE_BOUND);        //UART2
	Modules_Init();
}

