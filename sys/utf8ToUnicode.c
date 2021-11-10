#include <stdio.h>
#include <stdlib.h>
#include "utf8ToUnicode.h"
#include "GT5SLAD3B-GTA6401.h"

static int utf8ToUnicodeChar (unsigned char *ch, unsigned int *unicode)
{
    unsigned char *p = NULL;
    int e = 0, n = 0;
    p = ch;
    if(!p || !unicode) {
        return 0;
    }

    if(*p >= 0xfc) {
        /* 6:<11111100> */
        e = (p[0] & 0x01) << 30;
        e |= (p[1] & 0x3f) << 24;
        e |= (p[2] & 0x3f) << 18;
        e |= (p[3] & 0x3f) << 12;
        e |= (p[4] & 0x3f) << 6;
        e |= (p[5] & 0x3f);
        n = 6;
    } else if(*p >= 0xf8) {
        /* 5:<11111000> */
        e = (p[0] & 0x03) << 24;
        e |= (p[1] & 0x3f) << 18;
        e |= (p[2] & 0x3f) << 12;
        e |= (p[3] & 0x3f) << 6;
        e |= (p[4] & 0x3f);
        n = 5;
    } else if(*p >= 0xf0) {
        /* 4:<11110000> */
        e = (p[0] & 0x07) << 18;
        e |= (p[1] & 0x3f) << 12;
        e |= (p[2] & 0x3f) << 6;
        e |= (p[3] & 0x3f);
        n = 4;
    } else if(*p >= 0xe0) {
        /* 3:<11100000> */
        e = (p[0] & 0x0f) << 12;
        e |= (p[1] & 0x3f) << 6;
        e |= (p[2] & 0x3f);
        n = 3;
    } else if(*p >= 0xc0) {
        /* 2:<11000000> */
        e = (p[0] & 0x1f) << 6;
        e |= (p[1] & 0x3f);
        n = 2;
    } else {
        e = p[0];
        n = 1;
    }
    *unicode = e;

    /* Return bytes count of this utf-8 character */
    return n;
}

int utf8ToUnicode (unsigned char * utf8_str,
        unsigned int * unicode_str,
        int unicode_str_size)
{
    unsigned int unicode = 0;
    int n = 0;
    int count = 0;
    unsigned char *s = NULL;
    unsigned int *e = NULL;

    s = utf8_str;
    e = unicode_str;

    if (!utf8_str || !unicode_str)
        return 0;

    while (*s) {
        if ((n = utf8ToUnicodeChar (s, &unicode)) > 0) {
            if (++count  >= unicode_str_size) {
                return count;
            }
            else {
                *e = (unsigned int) unicode;
                e++;
                *e = 0;

                /* Step to next utf-8 character */
                s += n;
            }
        } else {
            /* Converting error occurs */
            return count;
        }
    }

    return count;
}

int utf8ToGBK (unsigned char * utf8_str, unsigned long * gbk_str, int gbk_str_size)
{
    unsigned int *temp_unicode_str = NULL;
    int ret_unicode_len = 0;
    int i;

    unsigned int *cur_temp_unicode_str = NULL;
    unsigned long *cur_gbk_str = NULL;

    if (!utf8_str || !gbk_str)
        return 0;

    if (gbk_str_size <= 0) {
        return 0;
    }

    temp_unicode_str = (unsigned int *)malloc(gbk_str_size * sizeof(unsigned int));
    if (NULL == temp_unicode_str) {
        return -1;
    }

    ret_unicode_len = utf8ToUnicode(utf8_str, temp_unicode_str, gbk_str_size);
    if (0 == ret_unicode_len) {
        return -2;
    }

    cur_temp_unicode_str = temp_unicode_str;
    cur_gbk_str = gbk_str;
    for (i=0; i<ret_unicode_len; i++) { 
		
		*cur_gbk_str = U2G(*cur_temp_unicode_str);
        cur_temp_unicode_str++;
        cur_gbk_str++;
    }

    free(temp_unicode_str);
    temp_unicode_str = NULL;

    return ret_unicode_len; /* gbk code number */
}
