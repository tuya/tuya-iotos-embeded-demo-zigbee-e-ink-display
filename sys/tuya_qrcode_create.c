/**
* @file tuya_qrcode_creat.c
* @author fudi.xu@tuya.com
* @brief tuya_qrcode_creat module is used to provide qrcode creat function
* @version 0.1
* @date 2021-08-24
*
* @copyright Copyright (c) tuya.inc 2021
*
*/
#include "qrcode_create.h"
#include "tuya_qrcode_create.h"
#include "USART.h"

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
OPERATE_RET tuya_svc_image_generate_qrcode_amplifier(TY_AMPLIFIE_IN_T *param_in,TY_AMPLIFIE_OUT_T **param_out) 
{
    unsigned char qrode_size = 0;
    qrode_size = param_in->src_side_length;
    TY_AMPLIFIE_OUT_T *temp = NULL;
    temp = (TY_AMPLIFIE_OUT_T *)malloc(sizeof(TY_AMPLIFIE_OUT_T));
    if(temp == NULL) {
        PR_ERR("malloc failed !!!!!!!!\n\r");
        return OPRT_COM_ERROR;
    }
    temp->dst_side_length  = param_in->src_side_length * param_in->magnifications;

    switch (param_in->mode) {

    case 0: {
        int h_byte_num = (param_in->magnifications*qrode_size + 7)/8;
        unsigned char *bit_data = NULL;
        bit_data = (unsigned char *)malloc(h_byte_num*qrode_size*param_in->magnifications);
        if(bit_data == NULL) {
            PR_ERR("malloc failed !!!!!!!!\n\r");
            return OPRT_COM_ERROR;
        }
        memset(bit_data,0,h_byte_num*qrode_size*param_in->magnifications);
        for (uint8_t y = 0; y < qrode_size; y++) {
            for (uint8_t x = 0; x < qrode_size; x++) {
                if (*(unsigned char*)param_in->src_data != 0) {
                    for(int i = 0;i<param_in->magnifications;i++) {
                        for(int j = 0;j<param_in->magnifications;j++) {
                            bit_data[(param_in->magnifications*y + i)*h_byte_num + (param_in->magnifications*x + j)/8] |= 1 << ((param_in->magnifications*x + j)%8);
                        }
                    }
                    
                } else {
                    for(int i = 0;i < param_in->magnifications;i++) {
                        for(int j = 0;j < param_in->magnifications;j++) {
                            bit_data[(param_in->magnifications*y + i)*h_byte_num + (param_in->magnifications*x + j)/8] &= ~(1 << ((param_in->magnifications*x + j)%8));
                        }
                    }
                }
                (unsigned char*)param_in->src_data++;
            }
            
        }
        temp->dst_data = bit_data;
        temp->dst_data_len = h_byte_num*qrode_size*param_in->magnifications;
        temp->h_byte_num = h_byte_num;
    }
    break;

    case 1: {
        unsigned char (*dst_data)[qrode_size*param_in->magnifications] = NULL;
        dst_data = (unsigned char(*)[qrode_size*param_in->magnifications])\
                    malloc(sizeof(unsigned char)*(qrode_size*param_in->magnifications)*(qrode_size*param_in->magnifications)); 
        if(dst_data == NULL) {
            PR_ERR("malloc failed !!!!!!!!\n\r");
            return OPRT_COM_ERROR;
        }
        memset(dst_data,0,sizeof(unsigned char)*(qrode_size*param_in->magnifications)*(qrode_size*param_in->magnifications));
        for (uint8_t y = 0; y < qrode_size; y++) {
            for (uint8_t x = 0; x < qrode_size; x++) {
                if (*(unsigned char*)param_in->src_data != 0) {
                    for(int i = 0;i<param_in->magnifications;i++) {
                        for(int j = 0;j<param_in->magnifications;j++) {
                            dst_data[param_in->magnifications*y + i][param_in->magnifications*x +j] = 0xff;
                        }
                    }
                    
                } else {
                    for(int i = 0;i < param_in->magnifications;i++) {
                        for(int j = 0;j < param_in->magnifications;j++) {
                            dst_data[param_in->magnifications*y + i][param_in->magnifications*x +j] = 0x00;
                        }
                    }
                }
                (unsigned char*)param_in->src_data++;
            }

        }
        temp->dst_data = (unsigned char*)dst_data;
        temp->dst_data_len = ((qrode_size*param_in->magnifications)*(qrode_size*param_in->magnifications));
        temp->h_byte_num =  temp->dst_side_length * sizeof(unsigned char);
    }
    break;
    default:
    break;
    };
    *param_out = temp;
    return OPRT_OK;
}

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
OPERATE_RET tuya_svc_image_generate_qrcode_create(TY_CREATE_IN_T* param_in,TY_CREATE_OUT_T** param_out) 
{

    // Create the QR code
    PR_DEBUG_RAW("Create the QR code\n\r");
    int ret = 0;
    QRCode qrcode;
    uint8_t qrcodeBytes[qrcode_getBufferSize(param_in->version)];
    ret = qrcode_initText(&qrcode, qrcodeBytes, param_in->version, param_in->ecc_level, param_in->information);
    if(ret != 0) {
        PR_ERR("qrcode init Text fail :%d",ret);
        return OPRT_COM_ERROR;
    }
    PR_DEBUG_RAW("QRcode Version :%d,ECC LEVEL:%d\n\r",qrcode.version,param_in->ecc_level);
    TY_CREATE_OUT_T* temp = NULL;
    temp = (TY_CREATE_OUT_T *)malloc(sizeof(TY_CREATE_OUT_T));
    if(temp == NULL) {
        PR_ERR("malloc failed !!!!!!!!\n\r");
        return OPRT_COM_ERROR;
    }
    switch (param_in->mode) {
    
    case 0: {
        int h_byte_num = (param_in->magnifications*qrcode.size + 7)/8;
        unsigned char *bit_data = NULL;
        bit_data = (unsigned char *)malloc(h_byte_num*qrcode.size*param_in->magnifications);
        if(bit_data == NULL) {
            PR_ERR("malloc failed !!!!!!!!\n\r");
            return OPRT_COM_ERROR;
        }
        memset(bit_data,0,h_byte_num*qrcode.size*param_in->magnifications);

        for (uint8_t y = 0; y < qrcode.size; y++) {
            for (uint8_t x = 0; x < qrcode.size; x++) {
                if (qrcode_getModule(&qrcode, x, y)) {
                    for(int i = 0;i<param_in->magnifications;i++) {
                        for(int j = 0;j<param_in->magnifications;j++) {
                            bit_data[(param_in->magnifications*y + i)*h_byte_num + (param_in->magnifications*x + j)/8] |= 1 << ((param_in->magnifications*x + j)%8);
                        }
                    }
                    
                    PR_DEBUG_RAW("&&");
                } else {
                    for(int i = 0;i < param_in->magnifications;i++) {
                        for(int j = 0;j < param_in->magnifications;j++) {
                            bit_data[(param_in->magnifications*y + i)*h_byte_num + (param_in->magnifications*x + j)/8] &= ~(1 << ((param_in->magnifications*x + j)%8));
                        }
                    }
                    PR_DEBUG_RAW("  ");
                }
            }
            
            PR_DEBUG_RAW("\n\r");
        }
        temp->dst_data = bit_data;
        temp->dst_side_length = qrcode.size*param_in->magnifications;
        temp->h_byte_num = h_byte_num;
        temp->dst_data_len = h_byte_num*qrcode.size*param_in->magnifications;
    }
    break;
    case 1: {
        unsigned char (*dst_data)[qrcode.size*param_in->magnifications] = NULL;
        dst_data = (unsigned char(*)[qrcode.size*param_in->magnifications])\
        malloc(sizeof(unsigned char)*(qrcode.size*param_in->magnifications)*(qrcode.size*param_in->magnifications)); 

        if(dst_data == NULL) {
            PR_ERR("malloc failed !!!!!!!!\n\r");
            return OPRT_COM_ERROR;
        }

        for (uint8_t y = 0; y < qrcode.size; y++) {
            for (uint8_t x = 0; x < qrcode.size; x++) {
                if (qrcode_getModule(&qrcode, x, y)) {

                    for(int i = 0;i<param_in->magnifications;i++) {
                        for(int j = 0;j<param_in->magnifications;j++) {
                            dst_data[param_in->magnifications*y + i][param_in->magnifications*x +j] = 0xff;
                        }
                    }
                    PR_DEBUG_RAW("&&");
                } else {
                    for(int i = 0;i < param_in->magnifications;i++) {
                        for(int j = 0;j < param_in->magnifications;j++) {
                            dst_data[param_in->magnifications*y + i][param_in->magnifications*x +j] = 0x00;
                        }
                    }
                    PR_DEBUG_RAW("  ");
                }
            }
            
            PR_DEBUG_RAW("\n\r");
        }
        temp->dst_data = (unsigned char*)dst_data;
        temp->dst_side_length = qrcode.size*param_in->magnifications;
        temp->h_byte_num = qrcode.size*param_in->magnifications * sizeof(char);
        temp->dst_data_len = (qrcode.size*param_in->magnifications)*(qrcode.size*param_in->magnifications);
    }
    break;
    default: 
    break;
    }
    *param_out = temp;

    return OPRT_OK;
}
/**
* @brief Release space for storing QR code data
*
* @param[in] data_addr,the qrcode data address
* @return success:0;fail:-1
*/
OPERATE_RET tuya_svc_image_generate_qrcode_create_free(TY_CREATE_OUT_T* data_addr)
{
    if(data_addr == NULL) {
        return OPRT_COM_ERROR;
    }
    free(data_addr->dst_data);
    free(data_addr);
    return OPRT_OK;

}


/**
* @brief Release space for storing Square image magnifying data
*
* @param[in] data_addr,the image data address
* @return success:0;fail:-1
*/
OPERATE_RET tuya_svc_image_generate_qrcode_amplifier_free(TY_AMPLIFIE_OUT_T* data_addr)
{
    if(data_addr == NULL) {
        return OPRT_COM_ERROR;
    }
    free(data_addr->dst_data);
    free(data_addr);
    return OPRT_OK;

}


