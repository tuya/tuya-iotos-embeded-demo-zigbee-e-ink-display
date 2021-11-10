#include "epaper.h"
extern unsigned char Epaper_border;
void Epaper_Spi_WriteByte(unsigned char TxData)
{				   			 
	unsigned char TempData;
	unsigned char scnt;
	TempData=TxData;
    EPD_W21_CLK_0;  
	for(scnt=0;scnt<8;scnt++)
	{ 
		if(TempData&0x80)
		  EPD_W21_MOSI_1 ;
		else
		  EPD_W21_MOSI_0 ;
		  EPD_W21_CLK_1;  
	      EPD_W21_CLK_0;  
		TempData=TempData<<1;
     }

}

uint8_t Epaper_READBUSY(void)
{ 
    uint8_t ret=1;
	uint16_t timeout=0xFFFF;
	LED2_SET;
	while(isEPD_W21_BUSY)
  {	 
		timeout--;
		if( 0 == timeout )
		{
			ret = 0;
			break;
		}			
  }
	return ret;
}

uint8_t Epaper_READBUSY_Long(void)
{ 
    uint8_t ret=1;
	uint16_t timeout=0xFF00;
	LED2_SET;
	while(isEPD_W21_BUSY)
  {
		timeout--;
		delay_ms(1);
		if( 0 == timeout )
		{
			ret = 0;
			break;
		}			
  }
	return ret;
}

void Epaper_Write_Command(unsigned char cmd)
{
	EPD_W21_CS_1;
	EPD_W21_CS_0;
	EPD_W21_DC_0;  // D/C#   0:command  1:data

	Epaper_Spi_WriteByte(cmd);
	EPD_W21_CS_1;
}

void Epaper_Write_Data(unsigned char data)
{
	EPD_W21_CS_1;
	EPD_W21_CS_0;
	EPD_W21_DC_1;  // D/C#   0:command  1:data

	Epaper_Spi_WriteByte(data);
	EPD_W21_CS_1;
}



/***********************************��ʼ��**********************************************/
// refresh_mode = Full ȫ��ˢ��
// refresh_mode = Partial  �ֲ�ˢ��

void EPD_HW_Init(const unsigned char refresh_mode)
{
	EPD_W21_Init();						
	Epaper_READBUSY();
	Epaper_Write_Command(0x12); 
	Epaper_READBUSY();
	Epaper_Write_Command(0x01);   
	Epaper_Write_Data(0x2B);
	Epaper_Write_Data(0x01);
	Epaper_Write_Data(0x00);
	Epaper_Write_Command(0x11);    
	Epaper_Write_Data(0x03);	
	Epaper_Write_Command(0x44);
	Epaper_Write_Data(0x00);		
	Epaper_Write_Data(0x31);   
	Epaper_Write_Command(0x45);        
	Epaper_Write_Data(0x00);   	
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x2B);	
	Epaper_Write_Data(0x01); 
	Epaper_Write_Command(0x3C); 
	Epaper_Write_Data(0x01);	
    Epaper_Write_Command(0x18);
	Epaper_Write_Data(0x80);	  
    Epaper_Write_Command(0x22); 
		if(refresh_mode==Full)
			Epaper_Write_Data(0xB1);  
		if(refresh_mode==Partial)
			Epaper_Write_Data(0xB9); 
    Epaper_Write_Command(0x20); 
    Epaper_READBUSY(); 	
	Epaper_Write_Command(0x4E);  
	Epaper_Write_Data(0x00);
	Epaper_Write_Command(0x4F);  
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x00);
	Epaper_READBUSY();	
}

void EPD_W21_Init(void)
{
	EPD_W21_RST_0;     
	driver_delay_xms(100); 
	EPD_W21_RST_1; 							//hard reset  
	driver_delay_xms(100); 
}


//////////////////////////////All screen update////////////////////////////////////////////
void EPD_WhiteScreen_ALL(const unsigned char *datas)
{
    unsigned int i;
    Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
    for(i=0;i<ALLSCREEN_GRAGHBYTES;i++)
    {               
     Epaper_Write_Data(*datas);
			datas++;
    }
    EPD_Update();	 
}

/////////////////////////////////////////////////////////////////////////////////////////
void EPD_Update(void)
{   
  Epaper_Write_Command(0x22); 
  Epaper_Write_Data(0xC7);   
  Epaper_Write_Command(0x20); 
  Epaper_READBUSY_Long();
}

void EPD_DeepSleep(void)
{  	
  Epaper_Write_Command(0x10); //enter deep sleep
  Epaper_Write_Data(0x01); 
  driver_delay_xms(100);
}
///////////////////////////Part update//////////////////////////////////////////////
//The x axis is reduced by one byte, and the y axis is reduced by one pixel.
void EPD_SetRAMValue_BaseMap( const unsigned char * datas)
{
	unsigned int i;   
	const unsigned char  *datas_flag;   
	datas_flag=datas;
    Epaper_Write_Command(0x24);   //Write Black and White image to RAM
    for(i=0;i<ALLSCREEN_GRAGHBYTES;i++)
    {               
     Epaper_Write_Data(*datas);
	 datas++;
    }
	 datas=datas_flag;
   Epaper_Write_Command(0x26);   //Write Black and White image to RAM
   for(i=0;i<ALLSCREEN_GRAGHBYTES;i++)
    {               
     Epaper_Write_Data(*datas);
			datas++;
    }
   EPD_Update();
}

//mode==POS , ���ԣ�
//mode==NEG , ���ԣ�
//mode==OFF , �����
//x��PART_LINE ������8����������
void EPD_Dis_Part(unsigned int xstart,unsigned int ystart,const unsigned char * datas,unsigned int PART_LINE,unsigned int PART_COLUMN,unsigned char mode)
{
	unsigned int i;  
	int xend,ystart_H,ystart_L,yend,yend_H,yend_L;
	xstart=xstart/8;//ת��Ϊ�ֽ�
	xend=xstart+PART_LINE/8-1; 
	ystart_H=ystart/256;
	ystart_L=ystart%256;
	yend=ystart+PART_COLUMN-1;
	yend_H=yend/256;
	yend_L=yend%256;		
	Epaper_Write_Command(0x44);       // set RAM x address start/end
	Epaper_Write_Data(xstart);    		// RAM x address start;
	Epaper_Write_Data(xend);    			// RAM x address end
	Epaper_Write_Command(0x45);       // set RAM y address start/end
	Epaper_Write_Data(ystart_L);    	// RAM y address start Low
	Epaper_Write_Data(ystart_H);    	// RAM y address start High
	Epaper_Write_Data(yend_L);    		// RAM y address end Low
	Epaper_Write_Data(yend_H);    		// RAM y address end High
	Epaper_Write_Command(0x4E);   		// set RAM x address count
	Epaper_Write_Data(xstart); 
	Epaper_Write_Command(0x4F);   		// set RAM y address count
	Epaper_Write_Data(ystart_L);
	Epaper_Write_Data(ystart_H);
	Epaper_Write_Command(0x24);        //Write Black and White image to RAM
		for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
			{                         
				if (mode==POS)
					{
						Epaper_Write_Data(*datas);
						datas++;
					}

				if (mode==NEG)
					{
						Epaper_Write_Data(~*datas);
						datas++;
					}	

			  if (mode==OFF)
				  {
						Epaper_Write_Data(0xFF);
					}		
				
			} 	

}
void EPD_Dis_Part2(unsigned int xstart,unsigned int ystart, unsigned char * datas,unsigned int PART_LINE,unsigned int PART_COLUMN)
{
	unsigned int i;  
	int xend,ystart_H,ystart_L,yend,yend_H,yend_L;
	xstart=xstart/8;
	xend=xstart+PART_LINE/8-1; 
	ystart_H=ystart/256;
	ystart_L=ystart%256;
	yend=ystart+PART_COLUMN-1;
	yend_H=yend/256;
	yend_L=yend%256;		
	Epaper_Write_Command(0x44);       // set RAM x address start/end
	Epaper_Write_Data(xstart);    		// RAM x address start;
	Epaper_Write_Data(xend);    			// RAM x address end
	Epaper_Write_Command(0x45);       // set RAM y address start/end
	Epaper_Write_Data(ystart_L);    	// RAM y address start Low
	Epaper_Write_Data(ystart_H);    	// RAM y address start High
	Epaper_Write_Data(yend_L);    		// RAM y address end Low
	Epaper_Write_Data(yend_H);    		// RAM y address end High
	Epaper_Write_Command(0x4E);   		// set RAM x address count
	Epaper_Write_Data(xstart); 
	Epaper_Write_Command(0x4F);   		// set RAM y address count
	Epaper_Write_Data(ystart_L);
	Epaper_Write_Data(ystart_H);
	Epaper_Write_Command(0x24);   //Write Black and White image to RAM
	for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
	{                         
		*datas = ((*datas & 0xf0) >> 4) | ((*datas & 0x0f) << 4);
		*datas = ((*datas & 0xcc) >> 2) | ((*datas & 0x33) << 2);
		*datas = ((*datas & 0xaa) >> 1) | ((*datas & 0x55) << 1);
		*datas=~(*datas);
		Epaper_Write_Data(*datas);
		datas++;
	} 	
}
void EPD_Part_Update(void)
{
	Epaper_Write_Command(0x22); 
	Epaper_Write_Data(0xCF);   
	Epaper_Write_Command(0x20); 
	Epaper_READBUSY(); 			
}

/////////////////////////////////Single display////////////////////////////////////////////////
void EPD_WhiteScreen_White(void)
{
    unsigned int k;
    Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)   36
		for(k=0;k<ALLSCREEN_GRAGHBYTES;k++)
		{
			Epaper_Write_Data(0xff);
		}
    Epaper_Write_Command(0x26);   //write RAM for black(0)/white (1)
		for(k=0;k<ALLSCREEN_GRAGHBYTES;k++)
		{
			Epaper_Write_Data(0xff);
		}	
		EPD_Update();
}


void EPD_White_Part(unsigned int xstart,unsigned int ystart,const unsigned char * datas,unsigned int PART_LINE,unsigned int PART_COLUMN,unsigned char mode)
{
	unsigned int i;  
	int xend,ystart_H,ystart_L,yend,yend_H,yend_L;
	xstart=xstart/8;//ת��Ϊ�ֽ�
	xend=xstart+PART_LINE/8-1; 
	ystart_H=ystart/256;
	ystart_L=ystart%256;
	yend=ystart+PART_COLUMN-1;
	yend_H=yend/256;
	yend_L=yend%256;		
	Epaper_Write_Command(0x44);       // set RAM x address start/end
	Epaper_Write_Data(xstart);    		// RAM x address start;
	Epaper_Write_Data(xend);    			// RAM x address end
	Epaper_Write_Command(0x45);       // set RAM y address start/end
	Epaper_Write_Data(ystart_L);    	// RAM y address start Low
	Epaper_Write_Data(ystart_H);    	// RAM y address start High
	Epaper_Write_Data(yend_L);    		// RAM y address end Low
	Epaper_Write_Data(yend_H);    		// RAM y address end High
	Epaper_Write_Command(0x4E);   		// set RAM x address count
	Epaper_Write_Data(xstart); 
	Epaper_Write_Command(0x4F);   		// set RAM y address count
	Epaper_Write_Data(ystart_L);
	Epaper_Write_Data(ystart_H);
	Epaper_Write_Command(0x24);   //Write Black and White image to RAM
		for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
			{                         
				if (mode==POS)
					{
						Epaper_Write_Data(*datas);
						datas++;
					}

				if (mode==NEG)
					{
						Epaper_Write_Data(~*datas);
						datas++;
					}	

			  if (mode==OFF)
				  {
						Epaper_Write_Data(0xFF);
					}		
				
			} 	

}


//////////////////////////////////////////////////////////////////////////////////////
