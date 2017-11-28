//
// Created by yanshuotang on 17/10/19.
//

#ifndef CP_CLASS_CPCLASS_H
#define CP_CLASS_CPCLASS_H

#include <stdio.h>
#include "common/common_type.h"
#include "utils/utf_8_tools/utf_8_tools.h"


#define JAVA_CLASSFILE_MAGIC              0xCAFEBABE
#define JAVA_MIN_SUPPORTED_VERSION        45
#define JAVA_MAX_SUPPORTED_VERSION        53
#define JAVA_MAX_SUPPORTED_MINOR_VERSION  0

// Used for two backward compatibility reasons:
// - to check for new additions to the class file format in JDK1.5
// - to check for bug fixes in the format checker in JDK1.5
#define JAVA_1_5_VERSION                  49

// Used for backward compatibility reasons:
// - to check for javac bug fixes that happened after 1.5
// - also used as the max version when running in jdk6
#define JAVA_6_VERSION                    50

// Used for backward compatibility reasons:
// - to disallow argument and require ACC_STATIC for <clinit> methods
#define JAVA_7_VERSION                    51

// Extension method support.
#define JAVA_8_VERSION                    52

#define JAVA_9_VERSION                    53



#define CP_OK 0 // 操作成功
#define CP_NO_DATA -1 // 没有数据
#define CP_NO_INIT -2 // 未初始化
#define CP_NO_MEM -3 //申请内存出错
#define CP_PARSE_CONSTANTS_ERROR -4 // 解析常量池错误

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u1* info;//info[attribute_length]
}attribute_info;

typedef struct  {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    attribute_info* attributes;
}method_info;


typedef struct {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    attribute_info* attributes;
}field_info;

typedef struct {
    u4             magic;
    u2             minor_version;
    u2             major_version;
    u2             constant_pool_count;
    CONSTANT_ENTITY* constant_pool; // 常量池
    u2             access_flags;
    u2             this_class;
    u2             super_class;
    u2             interfaces_count;
    u2*            interfaces;
    u2             fields_count;
    field_info*    fields;
    u2             methods_count;
    method_info*   methods;
    u2             attributes_count;
    attribute_info* attributes;
}CLASS;

typedef struct {
    FILE* fp;
    size_t len;
    size_t size;
    CLASS* aClass;
}CLASSFILE;



//打开并初始化，path为class文件的路经，CLASSFILE
CLASSFILE* class_open(const char *path);

int class_parse(CLASSFILE* classfile);


int do_parse(CLASSFILE* classfile,char *data);



#endif //CP_CLASS_CPCLASS_H
