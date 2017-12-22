//
// Created by yanshuotang on 17/11/15.
//

#include <string.h>
#include "utf_8_tools.h"

/**
 * 返回字节position位的二进制值
 * position为0~7位，左高位右低位
 * @param byte
 * @param position
 * @return
 */
int byte_position_bit(unsigned char byte,int position)
{
    int oc=1;
    oc<<=position;
    return (oc&byte)!=0?1:0;
}

unsigned int trans_two_byte(unsigned char* bytes){
    unsigned int result = 0;
    unsigned char x;
    unsigned char y;

    x = bytes[0];
    y = bytes[1];

//    printf("trans_two_byte_x=%x\n",x);
//    printf("trans_two_byte_y=%x\n",y);

    result = ((x & 0x1f) << 6) + (y & 0x3f);
    return result;

}

/**
 * UTF-8 code to unicode
 * @param byte1
 * @param byte2
 * @return
 */
wchar_t trans_two_bytes_wchar(u1 byte1, u1 byte2){
    wchar_t result = 0x0;
    result = ((byte1 & 0x1f) << 6) + (byte2 & 0x3f);
    return result;
}

wchar_t trans_three_bytes_wchar(u1 byte1,u1 byte2, u1 byte3){
    wchar_t result = 0x0;
    result = ((byte1 & 0xf) << 12) + ((byte2 & 0x3f) << 6 ) + (byte3 & 0x3f);
    return result;
}

wchar_t trans_six_bytes_wchar(u1 byte1,u1 byte2, u1 byte3, u1 byte4, u1 byte5, u1 byte6){
    wchar_t result = 0x0;
    result = 0x10000 + ((byte2 & 0x0f << 16) + ((byte3 & 0x3f) << 10) + (byte5 & 0x0f) << 6) + (byte6 & 0x3f);
    return result;
}


unsigned int trans_three_byte(unsigned char* bytes){
    unsigned int result = 0;
    unsigned char x;
    unsigned char y;
    unsigned char z;

    x = bytes[0];
    y = bytes[1];
    z = bytes[2];

//    printf("x=%x\n",x);
//    printf("y=%x\n",y);
//    printf("z=%x\n",z);

    result = ((x & 0xf) << 12) + ((y & 0x3f) << 6 ) + (z & 0x3f);

    return result;
}


unsigned int trans_six_bytes(unsigned char* bytes){
    unsigned int result = 0;

    unsigned char u;
    unsigned char v;
    unsigned char w;
    unsigned char x;
    unsigned char y;
    unsigned char z;
    u = bytes[0];
    v = bytes[1];
    w = bytes[2];
    x = bytes[3];
    y = bytes[4];
    z = bytes[5];

//    printf("trans_six_bytes_u=%x\n",u);
//    printf("trans_six_bytes_v=%x\n",v);
//    printf("trans_six_bytes_w=%x\n",w);
//    printf("trans_six_bytes_x=%x\n",x);
//    printf("trans_six_bytes_y=%x\n",y);
//    printf("trans_six_bytes_z=%x\n",z);

    result = 0x10000 + ((v & 0x0f << 16) + ((w & 0x3f) << 10) + (y & 0x0f) << 6) + (z & 0x3f);

    return result;
}


int get_utf_8_width(unsigned char byte){
    printf("byte:%x\n",byte);
    int bit_val = -1;
    // 0开头的则位1个字节宽的的UTF8
    bit_val = byte_position_bit(byte,7);
    if(0 == bit_val){
        return 1;
    }
    bit_val = byte_position_bit(byte,6);
    if(1 != bit_val){
        // 格式异常
        return -1;
    }
    // 110开头的则位2个字节宽度的UTF8
    bit_val = byte_position_bit(byte,5);
    if(0 == bit_val){
        // 2个字节
        return 2;
    }

    // 11101101格式的则位6个字节的补充字符
    if(0xED == byte){
        return 6;
    }

    // 1110开头的则为3个字节宽度的UTF8
    if(0 == byte_position_bit(byte,4)){
        return 3;
    }

    return 0;

}

int get_unicode_count(const unsigned char * bytes,int bytes_count){
    int unicode_count = 0;
    int bytes_position = 0;
    unsigned char ch = 0;
    int width = 0;
    redo:
    if(bytes_position +1 > bytes_count)
        goto end;
    ch = bytes[bytes_position];
    width = get_utf_8_width(ch);
    if(0 >= width){
        goto end;
    }
    unicode_count++;
    bytes_count -= width;
    if(0 < bytes_count){
        bytes_position += width;
        goto redo;
    }
    end:
    return unicode_count;
}


int get_unicode_by_wchar(const unsigned char bytes[], int bytes_count, wchar_t wchars[]){

    unsigned char bytes_copy[bytes_count];
    memset(bytes_copy,0,bytes_count);
    for(int i=0; i<bytes_count; i++){
        bytes_copy[i] = bytes[i];
    }

    int bytes_position = 0;
    int width = 0;
    int unicode_i = 0;
    unsigned char ch = 0;

    while (bytes_position < bytes_count){
        ch = bytes_copy[bytes_position];
        width = get_utf_8_width(ch);
        switch (width){
            case 1:
                wchars[unicode_i] = bytes_copy[bytes_position];
                unicode_i++;
                bytes_position++;
                break;
            case 2:
//                wchars[unicode_i] = trans_two_byte(bytes_copy);
                wchars[unicode_i] = trans_two_bytes_wchar(bytes_copy[bytes_position],bytes_copy[bytes_position+1]);
                unicode_i++;
                bytes_position+=2;
                break;
            case 3:
                wchars[unicode_i] = trans_three_bytes_wchar(bytes_copy[bytes_position],bytes_copy[bytes_position+1],
                                                          bytes_copy[bytes_position+2]);
                unicode_i++;
                bytes_position+=3;
                break;
            case 6:
                wchars[unicode_i] = trans_six_bytes_wchar(bytes_copy[bytes_position],bytes_copy[bytes_position+1],
                        bytes_copy[bytes_position+2],bytes_copy[bytes_position+3],bytes_copy[bytes_position+4],
                        bytes_copy[bytes_position+5]);
                unicode_i++;
                bytes_position+=6;
                break;
            default:
                printf("wrong utf-8 width:%d\n",width);
                return -1;
        }
    }


    return 0;
}

int getUnicodes(const unsigned char * bytes,int bytes_count, unsigned int* unicodes){

    unsigned char* bytes_copy = (unsigned char*)malloc(bytes_count);
    memset(bytes_copy,0,bytes_count);
    for(int i=0; i<bytes_count; i++){
        bytes_copy[i] = bytes[i];
    }

    int bytes_position = 0;
    int width = 0;
    int unicode_i = 0;
    unsigned char ch = 0;

    redo:
    if(bytes_position+1 > bytes_count){
        goto end;
    }
//    ch = bytes_copy[bytes_position];
//    ch = *(unsigned char*)bytes_copy;
    ch = bytes_copy[0];

    width = get_utf_8_width(ch);    // break point here,look the ch is the right code.
    // so i think the problem may be caused in parse utf-8 constant.
    switch (width){
        case 1:
//            unicodes[unicode_i] = *(unsigned char*)bytes_copy;
            unicodes[unicode_i] = bytes_copy[0];
            unicode_i++;
            bytes_position++;
            bytes_copy++;
            goto redo;
        case 2:
            unicodes[unicode_i] = trans_two_byte(bytes_copy);
            unicode_i++;
            bytes_position += 2;
            bytes_copy += 2;
            goto redo;
        case 3:
            unicodes[unicode_i] = trans_three_byte(bytes_copy);
            unicode_i++;
            bytes_position += 3;
            bytes_copy += 3;
            goto redo;
        case 6:
            unicodes[unicode_i] = trans_six_bytes(bytes_copy);
            unicode_i++;
            bytes_position += 6;
            bytes_copy += 6;
            goto redo;
        default:
            printf("wrong utf-8 width:%d\n",width);
            return -1;
    }

    end:
//    free(bytes_copy);
    return 0;
}