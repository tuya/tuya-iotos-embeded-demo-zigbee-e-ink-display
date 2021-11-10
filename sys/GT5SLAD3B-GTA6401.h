
#ifndef _GT5SLAD3B_GTA6401_H_
#define _GT5SLAD3B_GTA6401_H_
#include "SPI.h"
void ZK_TEST(void);
/* �ⲿ�������� */
//extern unsigned long r_dat_bat(unsigned long address,unsigned long DataLen,unsigned char *pBuff);
//extern unsigned char CheckID(unsigned char CMD, unsigned long address,unsigned long byte_long,unsigned char *p_arr);

/********************* ʸ�����ò��� *********************/
//����
#define VEC_SONG_STY		1		//����
#define VEC_FANG_STY		2		//����
#define VEC_BLACK_STY		3		//����
#define VEC_KAI_STY			4		//����
//ASCII��
#define VEC_FT_ASCII_STY 	5
#define VEC_DZ_ASCII_STY 	6
#define VEC_CH_ASCII_STY 	7
#define VEC_BX_ASCII_STY 	8
#define VEC_BZ_ASCII_STY 	9
#define VEC_FX_ASCII_STY 	10
#define VEC_GD_ASCII_STY 	11
#define VEC_HZ_ASCII_STY 	12
#define VEC_MS_ASCII_STY 	13
#define VEC_SX_ASCII_STY 	14
#define VEC_ZY_ASCII_STY 	15
#define VEC_TM_ASCII_STY 	16
//������
#define VEC_YJ_LATIN_STY	17

/******************* ���ֵ���ģʽ���� *******************/

/**
 * ��ʽһ VEC_ST_MODE : ͨ��ʹ������VECFONT_ST�ṹ�����, ���ýṹ����Ϣ,
 *   ��ȡ�������ݵ�zk_buffer[]������.
 * ��ʽ�� VEC_PARM_MODE : ͨ��ָ���������е���, ��ȡ�������ݵ�pBits[]������.
 * ps: ���ַ�ʽ��ͬʱ����ʹ��, ��һʹ�����.
*/
//#define VEC_ST_MODE
#define VEC_PARM_MODE

/********************* �ָ��� *********************/

#ifdef VEC_ST_MODE

    #define ZK_BUFFER_LEN   4608    //���޸Ĵ�С, Լ���� �ֺ�*�ֺ�/8.

    typedef struct vecFont
    {
        unsigned long fontCode;		//�ַ���������:GB18030, ASCII/����: unicode
        unsigned char type;			//����	@ʸ�����ò���
        unsigned char size;			//���ִ�С
        unsigned char thick;		//���ִ�ϸ
        unsigned char zkBuffer[ZK_BUFFER_LEN];	//���ݴ洢
    }VECFONT_ST;

    unsigned int get_font_st(VECFONT_ST * font_st);
#endif

#ifdef VEC_PARM_MODE
	/*
	 *��������	get_font()
	 *���ܣ�		ʸ�����ֶ�ȡ����
	 *������pBits		���ݴ洢
	 *		sty			��������ѡ��  @ʸ�����ò���
	 *		fontCode	�ַ���������:GB18030, ASCII/����: unicode
	 *		width		���ֿ��
	 *		height		���ָ߶�
	 *		thick		���ִ�ϸ
	 *����ֵ��������ʾ���
	**/
    unsigned int get_font(unsigned char *pBits,unsigned char sty,unsigned long fontCode,unsigned char width,unsigned char height, unsigned char thick);
#endif
/********************* ʸ��������� *********************/

/*
 *��������	get_Font_Gray()
 *����		�Ҷ�ʸ�����ֶ�ȡ����
 *������pBits		���ݴ洢
 *		sty			��������ѡ��  @ʸ�����ò���
 *		fontCode	�ַ���������:GB18030, ASCII/����: unicode
 *		fontSize	���ִ�С
 *		thick		���ִ�ϸ
 *����ֵ��re_buff[0] �ַ�����ʾ��� , re_buff[1] �ַ��ĻҶȽ׼�[1��/2��/3��/4��]
**/	
unsigned int* get_Font_Gray(unsigned char *pBits,unsigned char sty,unsigned long fontCode,unsigned char fontSize, unsigned char thick);

//UnicodeתGBK
unsigned long  U2G(unsigned int  unicode);	
//BIG5תGBK
unsigned int BIG52GBK( unsigned char h,unsigned char l );

/*----------------------------------------------------------------------------------------
 * �Ҷ�����ת������ 2�׻Ҷ�/4�׻Ҷ�
 * ˵�� : ����������ת��Ϊ�Ҷ����� [eg:32��������ת2�׻Ҷ�������תΪ16����Ҷ�����]
 * ���� ��
 *   OutPutData�Ҷ�����;	 width ���; High �߶�;	grade �ҶȽ׼�[1��/2��/3��/4��]
 *------------------------------------------------------------------------------------------*/
void Gray_Process(unsigned char *OutPutData ,int width,int High,unsigned char Grade);

/*----------------------------------------------------------------------------------------
 * �Ҷ�������ɫ���� 
 * BmpDst Ŀ��ͼƬ���� 
 * BmpSrc ͼ��ͼƬ���� 
 * WORD x, WORD y, ͼ����Ŀ��ͼƬ�� X,Yλ�á�
 * WORD src_w, WORD src_h,  ͼ��Ŀ�Ⱥ͸߶� 
 * WORD dst_w, WORD dst_h   Ŀ��ͼƬ�Ŀ�Ⱥ͸߶� 
 * SrcGray �Ҷ���������
 * Grade	�ҶȽ׼�[2��/4��]
 *------------------------------------------------------------------------------------------*/
void AlphaBlend_whiteBC(unsigned char *BmpDst,unsigned char *BmpSrc, int x, int y, 
	int src_w, int src_h, int dst_w, int dst_h,unsigned char *SrcGray,unsigned char Grade);

/*----------------------------------------------------------------------------------------
 * �Ҷ������뱳�����
 * BmpDst Ŀ��ͼƬ���� 
 * BmpSrc ͼ��ͼƬ���� 
 * WORD x, WORD y, ͼ����Ŀ��ͼƬ�� X,Yλ�á�
 * WORD src_w, WORD src_h,  ͼ��Ŀ�Ⱥ͸߶� 
 * WORD dst_w, WORD dst_h   Ŀ��ͼƬ�Ŀ�Ⱥ͸߶� 
 * SrcGray �Ҷ���������
 * Grade	�ҶȽ׼�[2��/4��]
 *------------------------------------------------------------------------------------------*/
void AlphaBlend_blackBC(unsigned char *BmpDst,unsigned char *BmpSrc, int x, int y,
	int src_w, int src_h, int dst_w, int dst_h,unsigned char *SrcGray,unsigned char Grade);

#endif

/*--------------------------------------- end of file ---------------------------------------------*/
