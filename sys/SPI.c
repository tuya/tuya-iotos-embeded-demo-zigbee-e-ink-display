#include "IO.h"
#include "SPI.h"
#include "GT5SLAD3B-GTA6401.h"
#include "epaper.h"
#include "string.h"
#include "utf8ToUnicode.h"

#define Rom_csH		LIB_CS_SET
#define Rom_csL 	LIB_CS_RESET
#define MOSIH 		SPI_MOSI_SET
#define MOSIL 		SPI_MOSI_RESET
#define Rom_sckH 	SPI_SCK_SET
#define Rom_sckL	SPI_SCK_RESET
#define MISO		SPI_MISO_STATE

TYPE_BUFFER_S FlashBuffer;

void zk_init(void)
{
  Rom_csH;
  MOSIH;
  Rom_sckH;
}

/*******************************************************************************/
// Send data sub-pro (STM8,STM32等双向口)   SPI发送地址的时序算法              /
/*******************************************************************************/

void Send_Byte(unsigned char out)
{
	unsigned char i = 0;
	for (i = 0; i < 8; i++) {
		Rom_sckL;    //字库芯片时钟置低
	  	if (((out<<i)&0x80)==0) {
		  MOSIL;
	  } else {
		MOSIH;
		Rom_sckH;
		} 
  }
}
 
/*******************************************************************************/
// Get data sub-pro (STM8,STM32��˫���)    SPI���յ������ݵ��㷨               /
/*******************************************************************************/
unsigned char Get_Byte(void)
{
	unsigned char i = 0;
	unsigned char read_dat;

	Rom_sckH;
	for (i = 0; i < 8; i++)
	{
		Rom_sckL;
		read_dat = read_dat<<1;
		if (MISO)
			read_dat|=0x01;
		else
			read_dat&=0xfe;
		Rom_sckH;
	}
	return(read_dat);
}
/*******************************************************************************/
//                    Send address sub-pro (STM8,STM32��51)                                     /
/*******************************************************************************/
void SPI_Address(unsigned char AddH,unsigned char AddM,unsigned char AddL) 
{
	Send_Byte(AddH);
	Send_Byte(AddM);
	Send_Byte(AddL);
}
/*******************************************************************************/
//                    Get N bytes sub-pro  (STM8,STM32��51)                                    //
/*******************************************************************************/
//�ͻ��Լ�ʵ�֣���address��ַ��ȡlen���ֽڵ����ݲ����뵽DZ_Data���鵱��
unsigned char r_dat_bat(unsigned long address,unsigned long DataLen,unsigned char *pBuff)
{
	unsigned long i;
    unsigned char addrHigh;
    unsigned char addrMid;
    unsigned char addrLow;
    addrHigh=address>>16;
    addrMid=address>>8;
    addrLow=(unsigned char)address;
  
	Rom_csL;          //Ƭѡѡ���ֿ�оƬ
	Send_Byte(0x03);	//��ͨ��ȡ������0X03,Ȼ���͵�ַ�߰�λaddrHigh���а�λaddrMid���Ͱ�λaddrLow��
	SPI_Address(addrHigh,addrMid,addrLow);	
	for (i = 0; i < DataLen; i++)
	*(pBuff+i)=Get_Byte();
	Rom_csH;
	return 0;
}

//�ͻ��Լ�ʵ�֣���address��ַ��ȡһ���ֽڵ����ݲ����ظ�����
unsigned char r_dat(uint32_t address){
	unsigned char buff;
	unsigned char addrHigh;
	unsigned char addrMid;
	unsigned char addrLow;
	addrHigh = address >> 16;
	addrMid = address >> 8;
	addrLow = (unsigned char)address;
	
	Rom_csL;
	Send_Byte(0x03);
	SPI_Address(addrHigh,addrMid,addrLow);
	buff = Get_Byte();
	Rom_csH;
	return buff;
}
/******************************************************
�ͻ��Լ�ʵ��, ���ļ������ڲ���Ҫ���øú���ƥ��оƬID��
����˵���ļ���ͷ�ļ��Ƿ���Ҫ, û�оͲ���Ҫʵ��
******************************************************/
unsigned char CheckID(unsigned char CMD, unsigned long address,unsigned long byte_long,unsigned char *p_arr)
{
	unsigned long j;
	Rom_csL;
	Send_Byte(CMD);
	Send_Byte((unsigned char)((address)>>16)); 
	Send_Byte((unsigned char)((address)>>8));   
	Send_Byte((unsigned char)address); 
	for (j = 0; j < byte_long; j++)
	{
		p_arr[j]=Get_Byte();
	}
	Rom_csH;
	return 1;
}


void GT5S_DeepSleep(void)
{ 
  Rom_csL; 	
	Send_Byte(0xB9);
	Rom_csH;
	delay_us(40);
}
void GT5S_Wakeup(void)
{ 
    Rom_csL; 	
	Send_Byte(0xAB);
	Rom_csH;
	delay_us(40);
}


unsigned char jtwb[128]="共享空间，预订优先";
unsigned char state[128]="暂不开放";
unsigned char pBits[512];
 

extern unsigned char utf8_code_buf[255];

void SEAT_SET(void)
{
    EPD_HW_Init(Partial);
	zk_init();
	GT5S_Wakeup();
	unsigned long gbk_m[10];
	utf8ToGBK(utf8_code_buf, gbk_m, 10);
	get_font(pBits,VEC_FT_ASCII_STY, FlashBuffer.seat_set[0],48,48,48);
	EPD_Dis_Part(8,45,pBits,48,48,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.seat_set[1],48,48,48);
	EPD_Dis_Part(40,45,pBits,48,48,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.seat_set[2],48,48,48);
	EPD_Dis_Part(64,45,pBits,48,48,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.seat_set[3],48,48,48);
	EPD_Dis_Part(88,45,pBits,48,48,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.seat_set[4],48,48,48);
	EPD_Dis_Part(114,45,pBits,48,48,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.seat_set[5],48,48,48);
	EPD_Dis_Part(136,45,pBits,48,48,NEG);
    //固定信息
	get_font(pBits,VEC_SONG_STY,(jtwb[0]<<8)+jtwb[1],24,24,24);
	EPD_Dis_Part(196,55,pBits,24,24,NEG);
	get_font(pBits,VEC_SONG_STY,(jtwb[2]<<8)+jtwb[3],24,24,24);
	EPD_Dis_Part(220,55,pBits,24,24,NEG);
	get_font(pBits,VEC_SONG_STY,(jtwb[4]<<8)+jtwb[5],24,24,24);
	EPD_Dis_Part(244,55,pBits,24,24,NEG);
	get_font(pBits,VEC_SONG_STY,(jtwb[6]<<8)+jtwb[7],24,24,24);
	EPD_Dis_Part(268,55,pBits,24,24,NEG);
	get_font(pBits,VEC_SONG_STY,(jtwb[8]<<8)+jtwb[9],24,24,24);
	EPD_Dis_Part(292,55,pBits,24,24,NEG);
	get_font(pBits,VEC_SONG_STY,(jtwb[10]<<8)+jtwb[11],24,24,24);
	EPD_Dis_Part(310,55,pBits,24,24,NEG);
	get_font(pBits,VEC_SONG_STY,(jtwb[12]<<8)+jtwb[13],24,24,24);
	EPD_Dis_Part(330,55,pBits,24,24,NEG);
	get_font(pBits,VEC_SONG_STY,(jtwb[14]<<8)+jtwb[15],24,24,24);
	EPD_Dis_Part(354,55,pBits,24,24,NEG);
	get_font(pBits,VEC_SONG_STY,(jtwb[16]<<8)+jtwb[17],24,24,24);
	EPD_Dis_Part(380,55,pBits,24,24,NEG);
	EPD_Part_Update();
	EPD_DeepSleep();
	GT5S_DeepSleep();
}

void SEAT_CLOSE(void)
{
    EPD_HW_Init(Partial);
	zk_init();
	GT5S_Wakeup();
	//暂不开放
	get_font(pBits,VEC_SONG_STY,(state[0]<<8)+state[1],48,48,48);    
	EPD_Dis_Part(48,170,pBits,48,48,NEG);	
	get_font(pBits,VEC_SONG_STY,(state[2]<<8)+state[3],48,48,48);    
	EPD_Dis_Part(96,170,pBits,48,48,NEG);
	get_font(pBits,VEC_SONG_STY,(state[4]<<8)+state[5],48,48,48);    
	EPD_Dis_Part(144,170,pBits,48,48,NEG);	
	get_font(pBits,VEC_SONG_STY,(state[6]<<8)+state[7],48,48,48);   
	EPD_Dis_Part(192,170,pBits,48,48,NEG);	
	EPD_Part_Update();
	EPD_DeepSleep();
	GT5S_DeepSleep();
}


void ST_ADD1(void)
{
	EPD_SetRAMValue_BaseMap(pBits);
	EPD_HW_Init(Partial);
	zk_init();
	GT5S_Wakeup();
    memset(pBits,0,sizeof(pBits));
    memcpy(pBits, pBits, sizeof(pBits));
	unsigned long gbk_n[30];
	utf8ToGBK(FlashBuffer.n_add, gbk_n, 30);
	//time:
	get_font(pBits,VEC_FT_ASCII_STY, FlashBuffer.st_add[0],24,24,24);    
	EPD_Dis_Part(0,150,pBits,24,24,NEG);  
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[1],24,24,24);    
	EPD_Dis_Part(16,150,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[2],24,24,24);    
	EPD_Dis_Part(32,150,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[3],24,24,24);   
	EPD_Dis_Part(44,150,pBits,24,24,NEG);	 
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[4],24,24,24);     
	EPD_Dis_Part(60,150,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,0X2D,24,24,24);  
	EPD_Dis_Part(80,150,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[0],24,24,24);  
	EPD_Dis_Part(96,150,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[1],24,24,24);    
	EPD_Dis_Part(112,150,pBits,24,24,NEG);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[2],24,24,24);   
	EPD_Dis_Part(130,150,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[3],24,24,24);    
	EPD_Dis_Part(140,150,pBits,24,24,NEG);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[4],24,24,24);   
	EPD_Dis_Part(153,150,pBits,24,24,NEG);	
   //name
	get_font(pBits,VEC_SONG_STY,gbk_n[0],24,24,24);  
	EPD_Dis_Part(176,150,pBits,24,24,NEG);	
	get_font(pBits,VEC_SONG_STY,gbk_n[1],24,24,24);    
	EPD_Dis_Part(200,150,pBits,24,24,NEG);	
	EPD_Part_Update();
	EPD_DeepSleep();
	GT5S_DeepSleep();
}
void ST_ADD2(void)
{
	EPD_SetRAMValue_BaseMap(pBits);            //更新第二条预约信息
	EPD_HW_Init(Partial);
	zk_init();
	GT5S_Wakeup();
    memset(pBits,0,sizeof(pBits));
    memcpy(pBits, pBits, sizeof(pBits));
	unsigned long gbk_n[30];
	utf8ToGBK(FlashBuffer.n_add, gbk_n, 30);
	//time
	get_font(pBits,VEC_FT_ASCII_STY, FlashBuffer.st_add[0],24,24,24);   
	EPD_Dis_Part(0,190,pBits,24,24,NEG);  
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[1],24,24,24);     
	EPD_Dis_Part(16,190,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[2],24,24,24);    
	EPD_Dis_Part(32,190,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[3],24,24,24);   
	EPD_Dis_Part(44,190,pBits,24,24,NEG);	 
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[4],24,24,24);   
	EPD_Dis_Part(60,190,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,0X2D,24,24,24);
	EPD_Dis_Part(80,190,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[0],24,24,24);     
	EPD_Dis_Part(96,190,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[1],24,24,24);     
	EPD_Dis_Part(112,190,pBits,24,24,NEG);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[2],24,24,24);      
	EPD_Dis_Part(130,190,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[3],24,24,24);     
	EPD_Dis_Part(140,190,pBits,24,24,NEG);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[4],24,24,24); 
	EPD_Dis_Part(152,190,pBits,24,24,NEG);	
   //name
	get_font(pBits,VEC_SONG_STY,gbk_n[0],24,24,24);
	EPD_Dis_Part(176,190,pBits,24,24,NEG);	
	get_font(pBits,VEC_SONG_STY,gbk_n[1],24,24,24);
	EPD_Dis_Part(200,190,pBits,24,24,NEG);
	EPD_Part_Update();
	EPD_DeepSleep();
	GT5S_DeepSleep();

}
void ST_ADD3(void)
{
	EPD_SetRAMValue_BaseMap(pBits);            //更新第三条预约信息
	EPD_HW_Init(Partial);
	zk_init();
	GT5S_Wakeup();
    memset(pBits,0,sizeof(pBits));
    memcpy(pBits, pBits, sizeof(pBits));
	unsigned long gbk_n[30];
	utf8ToGBK(FlashBuffer.n_add, gbk_n, 30);   
	//time
	get_font(pBits,VEC_FT_ASCII_STY, FlashBuffer.st_add[0],24,24,24);  
	EPD_Dis_Part(0,230,pBits,24,24,NEG);  
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[1],24,24,24);    
	EPD_Dis_Part(16,230,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[2],24,24,24);   
	EPD_Dis_Part(32,230,pBits,24,24,NEG);		           
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[3],24,24,24);
	EPD_Dis_Part(44,230,pBits,24,24,NEG);	 
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[4],24,24,24); 
	EPD_Dis_Part(60,230,pBits,24,24,NEG);	            
	get_font(pBits,VEC_FT_ASCII_STY,0X2D,24,24,24);  
	EPD_Dis_Part(80,230,pBits,24,24,NEG);
	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[0],24,24,24);  
	EPD_Dis_Part(96,230,pBits,24,24,NEG);	            
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[1],24,24,24); 
	EPD_Dis_Part(112,230,pBits,24,24,NEG);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[2],24,24,24);  
	EPD_Dis_Part(130,230,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[3],24,24,24); 
	EPD_Dis_Part(140,230,pBits,24,24,NEG);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[4],24,24,24);
	EPD_Dis_Part(152,230,pBits,24,24,NEG);	
   //name
	get_font(pBits,VEC_SONG_STY,gbk_n[0],24,24,24);    
	EPD_Dis_Part(176,230,pBits,24,24,NEG);	
	get_font(pBits,VEC_SONG_STY,gbk_n[1],24,24,24);  
	EPD_Dis_Part(200,230,pBits,24,24,NEG);		
	EPD_Part_Update();
	EPD_DeepSleep();
	GT5S_DeepSleep();

}
void ST_DEL1(void)
{
    EPD_SetRAMValue_BaseMap(pBits);                 //删除该条信息
    EPD_HW_Init(Partial); 	         
	zk_init();
	GT5S_Wakeup();
	memcpy(pBits, pBits, sizeof(pBits));
	unsigned long gbk_n[30];
	utf8ToGBK(FlashBuffer.n_add, gbk_n, 30);
	get_font(pBits,VEC_FT_ASCII_STY, FlashBuffer.st_add[0],24,24,24);     //1
	EPD_Dis_Part(0,150,pBits,24,24,OFF);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[1],24,24,24);     //4
	EPD_Dis_Part(16,150,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[2],24,24,24);      //:
	EPD_Dis_Part(32,150,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[3],24,24,24);     //3
	EPD_Dis_Part(44,150,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[4],24,24,24);     //0
	EPD_Dis_Part(60,150,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,0x2D,24,24,24);     //-
	EPD_Dis_Part(80,150,pBits,24,24,OFF);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[0],24,24,24);      //1
	EPD_Dis_Part(96,150,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[1],24,24,24);     //6
	EPD_Dis_Part(112,150,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[2],24,24,24);     //:
	EPD_Dis_Part(130,150,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[3],24,24,24);     //3
	EPD_Dis_Part(140,150,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[4],24,24,24);     //0
	EPD_Dis_Part(153,150,pBits,24,24,OFF);	
	get_font(pBits,VEC_SONG_STY,gbk_n[0],24,24,24); 
	EPD_Dis_Part(176,150,pBits,24,24,OFF); 
	get_font(pBits,VEC_SONG_STY,gbk_n[1],24,24,24); 
	EPD_Dis_Part(200,150,pBits,24,24,OFF);
	EPD_Part_Update();
	EPD_DeepSleep();
	GT5S_DeepSleep();
}
void ST_DEL2(void)
{
    EPD_SetRAMValue_BaseMap(pBits);  
    EPD_HW_Init(Partial); 	         
	zk_init();
	GT5S_Wakeup();
	memcpy(pBits, pBits, sizeof(pBits));
	unsigned long gbk_n[30];
	utf8ToGBK(FlashBuffer.n_add, gbk_n, 30);
	get_font(pBits,VEC_FT_ASCII_STY, FlashBuffer.st_add[0],24,24,24);     //1
	EPD_Dis_Part(0,190,pBits,24,24,OFF);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[1],24,24,24);     //4
	EPD_Dis_Part(16,190,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[2],24,24,24);      //:
	EPD_Dis_Part(32,190,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[3],24,24,24);     //3
	EPD_Dis_Part(44,190,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[4],24,24,24);     //0
	EPD_Dis_Part(60,190,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,0x2D,24,24,24);     //-
	EPD_Dis_Part(80,190,pBits,24,24,OFF);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[0],24,24,24);      //1
	EPD_Dis_Part(96,190,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[1],24,24,24);     //6
	EPD_Dis_Part(112,190,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[2],24,24,24);     //:
	EPD_Dis_Part(130,190,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[3],24,24,24);     //3
	EPD_Dis_Part(140,190,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[4],24,24,24);     //0
	EPD_Dis_Part(152,190,pBits,24,24,OFF);	
	get_font(pBits,VEC_SONG_STY,gbk_n[0],24,24,24);  
	EPD_Dis_Part(176,190,pBits,24,24,OFF); 
	get_font(pBits,VEC_SONG_STY,gbk_n[1],24,24,24); 
	EPD_Dis_Part(200,190,pBits,24,24,OFF);
	EPD_Part_Update();
	EPD_DeepSleep();
	GT5S_DeepSleep();

}


void ST_DEL3(void)
{
    EPD_SetRAMValue_BaseMap(pBits);  
    EPD_HW_Init(Partial); 	         ////删除该条信息
	zk_init();
	GT5S_Wakeup();
	memcpy(pBits, pBits, sizeof(pBits));
	unsigned long gbk_n[30];
	utf8ToGBK(FlashBuffer.n_add, gbk_n, 30);
	get_font(pBits,VEC_FT_ASCII_STY, FlashBuffer.st_add[0],24,24,24);     //1
	EPD_Dis_Part(0,230,pBits,24,24,OFF);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[1],24,24,24);     //4
	EPD_Dis_Part(16,230,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[2],24,24,24);      //:
	EPD_Dis_Part(32,230,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[3],24,24,24);     //3
	EPD_Dis_Part(44,230,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_add[4],24,24,24);     //0
	EPD_Dis_Part(60,230,pBits,24,24,OFF);		
	get_font(pBits,VEC_FT_ASCII_STY,0x2D,24,24,24);     //-
	EPD_Dis_Part(80,230,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[0],24,24,24);      //1
	EPD_Dis_Part(96,230,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[1],24,24,24);     //6
	EPD_Dis_Part(112,230,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[2],24,24,24);     //:
	EPD_Dis_Part(130,230,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[3],24,24,24);     //3
	EPD_Dis_Part(140,230,pBits,24,24,OFF);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_add[4],24,24,24);     //0
	EPD_Dis_Part(152,230,pBits,24,24,OFF);	
	get_font(pBits,VEC_SONG_STY,gbk_n[0],24,24,24); 
	EPD_Dis_Part(176,230,pBits,24,24,OFF); 
	get_font(pBits,VEC_SONG_STY,gbk_n[1],24,24,24);  
	EPD_Dis_Part(200,230,pBits,24,24,OFF);
	EPD_Part_Update();
	EPD_DeepSleep();
	GT5S_DeepSleep();
}


void ST_ALL1(void)
{
    EPD_HW_Init(Partial);
	zk_init();
	GT5S_Wakeup();
	unsigned long gbk_1[30];
	utf8ToGBK(FlashBuffer.n_all, gbk_1, 30);
	//time
	get_font(pBits,VEC_FT_ASCII_STY, FlashBuffer.st_all[0],24,24,24);   
	EPD_Dis_Part(0,150,pBits,24,24,NEG);  
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_all[1],24,24,24);   
	EPD_Dis_Part(16,150,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_all[2],24,24,24);    
	EPD_Dis_Part(32,150,pBits,24,24,NEG);		          
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_all[3],24,24,24);   
	EPD_Dis_Part(44,150,pBits,24,24,NEG);	 
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_all[4],24,24,24);   
	EPD_Dis_Part(60,150,pBits,24,24,NEG);	            
	get_font(pBits,VEC_FT_ASCII_STY,0X2D,24,24,24);  
	EPD_Dis_Part(80,150,pBits,24,24,NEG);	            
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_all[0],24,24,24);    
	EPD_Dis_Part(96,150,pBits,24,24,NEG);	            
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_all[1],24,24,24);      
	EPD_Dis_Part(112,150,pBits,24,24,NEG);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_all[2],24,24,24);   
	EPD_Dis_Part(130,150,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_all[3],24,24,24);  
	EPD_Dis_Part(140,150,pBits,24,24,NEG);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_all[4],24,24,24);    
	EPD_Dis_Part(153,150,pBits,24,24,NEG);	
	//name
	get_font(pBits,VEC_SONG_STY,gbk_1[0],24,24,24);  
	EPD_Dis_Part(176,150,pBits,24,24,NEG);	
	get_font(pBits,VEC_SONG_STY,gbk_1[1],24,24,24); 
	EPD_Dis_Part(200,150,pBits,24,24,NEG);	
	EPD_Part_Update();
	EPD_DeepSleep();
	GT5S_DeepSleep();
}


//同步第二条预约信息
void ST_ALL2(void)
{
    EPD_HW_Init(Partial);
	zk_init();
	GT5S_Wakeup();
	unsigned long gbk_1[30];
	utf8ToGBK(FlashBuffer.n_all, gbk_1, 30);
	//time
	get_font(pBits,VEC_FT_ASCII_STY, FlashBuffer.st_all[0],24,24,24);   
	EPD_Dis_Part(0,190,pBits,24,24,NEG);  
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_all[1],24,24,24);   
	EPD_Dis_Part(16,190,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_all[2],24,24,24);  
	EPD_Dis_Part(32,190,pBits,24,24,NEG);		        
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_all[3],24,24,24);  
	EPD_Dis_Part(44,190,pBits,24,24,NEG);	 
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_all[4],24,24,24);  
	EPD_Dis_Part(60,190,pBits,24,24,NEG);	            
	get_font(pBits,VEC_FT_ASCII_STY,0X2D,24,24,24);    
	EPD_Dis_Part(80,190,pBits,24,24,NEG);	            
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_all[0],24,24,24);   
	EPD_Dis_Part(96,190,pBits,24,24,NEG);	            
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_all[1],24,24,24);   
	EPD_Dis_Part(112,190,pBits,24,24,NEG);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_all[2],24,24,24);     
	EPD_Dis_Part(130,190,pBits,24,24,NEG);
	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_all[3],24,24,24);   
	EPD_Dis_Part(140,190,pBits,24,24,NEG);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_all[4],24,24,24);     
	EPD_Dis_Part(152,190,pBits,24,24,NEG);	
	//name
	get_font(pBits,VEC_SONG_STY,gbk_1[0],24,24,24);   
	EPD_Dis_Part(176,190,pBits,24,24,NEG);	
	get_font(pBits,VEC_SONG_STY,gbk_1[1],24,24,24);   
	EPD_Dis_Part(200,190,pBits,24,24,NEG);	
	EPD_Part_Update();
	EPD_DeepSleep();
	GT5S_DeepSleep();
}
//同步第三条预约信息
void ST_ALL3(void)
{
    EPD_HW_Init(Partial);
	zk_init();
	GT5S_Wakeup();
	unsigned long gbk_1[30];
	utf8ToGBK(FlashBuffer.n_all, gbk_1, 30);
	//time
	get_font(pBits,VEC_FT_ASCII_STY, FlashBuffer.st_all[0],24,24,24);   
	EPD_Dis_Part(0,230,pBits,24,24,NEG);  
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_all[1],24,24,24);     
	EPD_Dis_Part(16,230,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_all[2],24,24,24);
	EPD_Dis_Part(32,230,pBits,24,24,NEG);		      
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_all[3],24,24,24); 
	EPD_Dis_Part(44,230,pBits,24,24,NEG);	 
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.st_all[4],24,24,24); 
	EPD_Dis_Part(60,230,pBits,24,24,NEG);	            
	get_font(pBits,VEC_FT_ASCII_STY,0X2D,24,24,24); 
	EPD_Dis_Part(80,230,pBits,24,24,NEG);	            
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_all[0],24,24,24);     
	EPD_Dis_Part(96,230,pBits,24,24,NEG);	            
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_all[1],24,24,24);    
	EPD_Dis_Part(112,230,pBits,24,24,NEG);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_all[2],24,24,24);   
	EPD_Dis_Part(130,230,pBits,24,24,NEG);
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_all[3],24,24,24);    
	EPD_Dis_Part(140,230,pBits,24,24,NEG);	
	get_font(pBits,VEC_FT_ASCII_STY,FlashBuffer.et_all[4],24,24,24);  
	EPD_Dis_Part(152,230,pBits,24,24,NEG);	
	//name
	get_font(pBits,VEC_SONG_STY,gbk_1[0],24,24,24);    
	EPD_Dis_Part(176,230,pBits,24,24,NEG);	
	get_font(pBits,VEC_SONG_STY,gbk_1[1],24,24,24);   
	EPD_Dis_Part(200,230,pBits,24,24,NEG);	
	EPD_Part_Update();
	EPD_DeepSleep();
	GT5S_DeepSleep();
}









