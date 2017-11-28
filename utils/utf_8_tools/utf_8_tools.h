//
// Created by yanshuotang on 17/11/15.
//

#ifndef CP_CLASS_UTF_8_TOOLS_H
#define CP_CLASS_UTF_8_TOOLS_H
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

unsigned int trans_two_byte(unsigned char* bytes);

unsigned int trans_three_byte(unsigned char* bytes);

unsigned int trans_six_bytes(unsigned char* bytes);

/**
 * 根据首字节判断接下来字符的宽度
 * @param byte
 * @return
 */
int get_utf_8_width(unsigned char byte);

int get_unicode_count(unsigned char * bytes,int bytes_count);

int getUnicodes(unsigned char * bytes,int bytes_count, unsigned int* unicodes);




#endif //CP_CLASS_UTF_8_TOOLS_H
