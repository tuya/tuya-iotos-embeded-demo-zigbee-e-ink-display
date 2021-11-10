/**
* @file tuya_qrcode_create.h
* @author fudi.xu@tuya.com
* @brief tuya_qrcode_create module is used to qrocde create
* @version 0.1
* @date 2021-08-24
*
* @copyright Copyright (c) tuya.inc 2021
*
*/
 
#ifndef __TUYA_QRCODE_CREATE_H__
#define __TUYA_QRCODE_CREATE_H__

 
#ifdef __cplusplus
extern "C" {
#endif
//#include "tuya_error_code.h"
#define PR_ERR printf
#define PR_DEBUG_RAW printf
#define PR_DEBUG printf
#define OPERATE_RET int
#define	OPRT_COM_ERROR -1
#define	OPRT_OK 0
/***********************************************************
*************************micro define***********************
***********************************************************/
typedef unsigned char QRCODE_ECC_LEVEL;    //四个纠错等级
#define QRCODE_ECC_LOW       0
#define QRCODE_ECC_MEDIUM    1
#define QRCODE_ECC_QUARTILE  2
#define QRCODE_ECC_HIGH      3

typedef unsigned char STORAGE_MODE;        //图像数据存储模式
#define STORAGE_MODE_BIT       0           //1bit存储一个像素
#define STORAGE_MODE_BYTE      1           //1byte存储一个像素
/***********************************************************
***********************typedef define***********************
***********************************************************/
 
typedef struct {
    unsigned char *src_data;    // Raw data to enlarge
    int magnifications;         // The multiple of the image to be magnified
    int src_side_length;            // The edge length of the incoming square image
    STORAGE_MODE mode;         //Determine the format of the output data;mode = 1:1byte->1 image pixel  mode = 0:1bit->1 image pixel 
}TY_AMPLIFIE_IN_T;

typedef struct {
    int dst_side_length;          // The edge length of the outgoing square image
    int dst_data_len;           // The size of the enlarged image data   byte
    int h_byte_num;             // The number of bytes occupied by a line of pixels
    unsigned char *dst_data;   //Point to the first address of the amplified data
}TY_AMPLIFIE_OUT_T;

typedef struct {
    unsigned char version;          // 1-40,The version of a QR code 
    unsigned char magnifications;   // magnification  For example, 21*21 -> magnifications*21 * magnifications*21
    QRCODE_ECC_LEVEL ecc_level;        //ECC_LOW ECC_MEDIUM ECC_QUARTILE ECC_HIGH
    STORAGE_MODE mode;             //Determine the format of the output data;mode = 1:1byte->1 image pixel  mode = 0:1bit->1 image pixel 
    char *information;              //Qr code to generate information,For example: www.tuya.com
}TY_CREATE_IN_T;

typedef struct {
    int dst_side_length;            // The edge length of the outgoing square image
    int dst_data_len;               // The size of the enlarged image data   byte
    int h_byte_num;                 // The number of bytes occupied by a line of pixels
    unsigned char *dst_data;        //Point to the first address of the dst data
}TY_CREATE_OUT_T;
/***********************************************************
***********************variable define**********************
***********************************************************/
 
 
/***********************************************************
***********************function define**********************
***********************************************************/
/**
* @brief Square image magnifying interface 
*
* @param[in] param_in,
    typedef struct {
*     unsigned char *src_data;    // Raw data to enlarge
*     int magnifications;         // The multiple of the image to be magnified
*     int src_side_length;            // The edge length of the incoming square image
*     STORAGE_MODE mode;         //Determine the format of the output data;mode = 1:1byte->1 image pixel  mode = 0:1bit->1 image pixel 
    }TY_AMPLIFIE_IN_T;
* @param[out] param_out,
    typedef struct {
            int dst_side_length;          // The edge length of the outgoing square image
            int dst_data_len;           // The size of the enlarged image data   byte
            int h_byte_num;             // The number of bytes occupied by a line of pixels
            unsigned char *dst_data;   //Point to the first address of the amplified data
    }TY_AMPLIFIE_OUT_T;
* @return success:OPRT_OK,fail:OPRT_COM_ERROR
*/
OPERATE_RET tuya_svc_image_generate_qrcode_amplifier(TY_AMPLIFIE_IN_T *param_in,TY_AMPLIFIE_OUT_T **param_out);

/**
* @brief generate_qrcode function
*
* @param[in] param_in,
typedef struct {
    unsigned char version;          // 1-40,The version of a QR code 
    unsigned char magnifications;   // magnification  For example, 21*21 -> magnifications*21 * magnifications*21
    QRCODE_ECC_LEVEL ecc_level;        //ECC_LOW ECC_MEDIUM ECC_QUARTILE ECC_HIGH
    STORAGE_MODE mode;             //Determine the format of the output data;mode = 1:1byte->1 image pixel  mode = 0:1bit->1 image pixel 
    char *information;              //Qr code to generate information,For example: www.tuya.com
}TY_CREATE_IN_T;
* @param[out] param_out,
typedef struct {
    int dst_side_length;            // The edge length of the outgoing square image
    int dst_data_len;               // The size of the enlarged image data   byte
    int h_byte_num;                 // The number of bytes occupied by a line of pixels
    unsigned char *dst_data;        //Point to the first address of the dst data
}TY_CREATE_IN_T;
* @return success:OPRT_OK;fail:OPRT_COM_ERROR
*/
OPERATE_RET tuya_svc_image_generate_qrcode_create(TY_CREATE_IN_T* param_in,TY_CREATE_OUT_T** param_out); 

/**
* @brief Release space for storing QR code data
*
* @param[in] data_addr,TY_CREATE_OUT_T data address
* @return success:0;fail:-1
*/
OPERATE_RET tuya_svc_image_generate_qrcode_create_free(TY_CREATE_OUT_T* data_addr);

/**
* @brief Release space for storing Square image magnifying data
*
* @param[in] data_addr,the image data address
* @return success:0;fail:-1
*/
OPERATE_RET tuya_svc_image_generate_qrcode_amplifier_free(TY_AMPLIFIE_OUT_T* data_addr);
OPERATE_RET tuya_svc_image_generate_qrcode_create_test(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__TUYA_QRCODE_CREATE_H__*/
