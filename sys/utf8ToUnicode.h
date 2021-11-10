#ifndef __UTF8_TO_UNICODE_H__
#define __UTF8_TO_UNICODE_H__

/**
* @brief 
*
* @param[in] utf8_str: utf8 string
* @param[out] unicode_str: unicode string
* @param[in] unicode_str_size: unicode string len
* @return change string len
*/
int utf8ToUnicode (unsigned char * utf8_str, unsigned int * unicode_str, int unicode_str_size);

/**
* @brief 
*
* @param[in] utf8_str: utf8 string
* @param[out] gbk_str: unicode string
* @param[in] gbk_str_size: gbk string len
* @return change string len
*/
int utf8ToGBK (unsigned char * utf8_str, unsigned long * gbk_str, int gbk_str_size);

#endif /* __UTF8_TO_UNICODE_H__ */
