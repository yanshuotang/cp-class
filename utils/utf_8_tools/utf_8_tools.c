//
// Created by yanshuotang on 17/11/15.
//

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

int get_unicode_count(unsigned char * bytes,int bytes_count){
    int unicode_count = 0;
    int bytes_position = 0;
    unsigned char ch = 0;
    int width = 0;
    redo:
    ch = bytes[bytes_position];
    width = get_utf_8_width(ch);
    if(0 >= width){
        return width;
    }
    unicode_count++;
    bytes_count -= width;
    if(0 < bytes_count){
        bytes_position += width;
        goto redo;
    }
    return unicode_count;
}

int getUnicodes(unsigned char * bytes,int bytes_count, unsigned int* unicodes){
    int bytes_position = 0;
    int width = 0;
    int unicode_i = 0;
    unsigned char ch = 0;

    redo:
    if(bytes_position+1 > bytes_count){
        goto end;
    }
    ch = bytes[bytes_position];
    width = get_utf_8_width(ch);
    switch (width){
        case 1:
            unicodes[unicode_i] = *(unsigned char*)bytes;
            unicode_i++;
            bytes_position++;
            bytes++;
            goto redo;
        case 2:
            unicodes[unicode_i] = trans_two_byte(bytes);
            unicode_i++;
            bytes_position += 2;
            bytes += 2;
            goto redo;
        case 3:
            unicodes[unicode_i] = trans_three_byte(bytes);
            unicode_i++;
            bytes_position += 3;
            bytes += 3;
            goto redo;
        case 6:
            unicodes[unicode_i] = trans_six_bytes(bytes);
            unicode_i++;
            bytes_position += 6;
            bytes += 6;
            goto redo;
        default:
            return -1;
    }

    end:
    return 0;
}