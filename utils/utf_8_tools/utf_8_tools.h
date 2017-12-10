//
// Created by yanshuotang on 17/11/15.
//

#ifndef CP_CLASS_UTF_8_TOOLS_H
#define CP_CLASS_UTF_8_TOOLS_H
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include "../../common/common_type.h"

unsigned int trans_two_byte(unsigned char* bytes);

unsigned int trans_three_byte(unsigned char* bytes);

unsigned int trans_six_bytes(unsigned char* bytes);

/**
 * 根据首字节判断接下来字符的宽度
 * @param byte
 * @return
 */
int get_utf_8_width(unsigned char byte);

int get_unicode_count(const unsigned char * bytes,int bytes_count);

int getUnicodes(const unsigned char * bytes,int bytes_count, unsigned int* unicodes);

int get_unicode_by_wchar(const unsigned char bytes[], int bytes_count, wchar_t wchars[]);


wchar_t trans_two_bytes_wchar(u1 byte1, u1 byte2);

wchar_t trans_three_bytes_wchar(u1 byte1,u1 byte2, u1 byte3);

wchar_t trans_six_bytes_wchar(u1 byte1,u1 byte2, u1 byte3, u1 byte4, u1 byte5, u1 byte6);




#endif //CP_CLASS_UTF_8_TOOLS_H
