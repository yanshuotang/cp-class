//
// Created by yanshuotang on 17/10/19.
//
#include <stdlib.h>
#include "cpclass.h"
#include "endian_swap/endian_swap.h"
#include "class_files/constant/class_constants.h"

CLASSFILE* class_open(const char *path){
    CLASSFILE* classfile;
    classfile = malloc(sizeof(CLASSFILE));
    if(NULL == classfile)
    {
        printf("申请内存空间出错!\n");
        return NULL;
    }

    if((classfile->fp = fopen(path,"rb")) == NULL){
        printf("打开%s出错!\n",path);
        return NULL;
    }

    classfile->len = 0;
    classfile->aClass = NULL;

    return classfile;
}

int class_parse(CLASSFILE* classfile){
    char* data; // class文件内容
    long file_len; // class文件长度
    int res_code;

    if(fseek(classfile->fp,0L,SEEK_END) != 0)
        return CP_NO_INIT;
    file_len=ftell(classfile->fp);
    rewind(classfile->fp);
    data=malloc(sizeof(char)*file_len+1);
    if(NULL == data){
        printf("NO_MEMERY!");
        return CP_NO_MEM;
    }
    data[file_len]='\0';
    fread(data,file_len,1,classfile->fp);
    fclose(classfile->fp);

    classfile->aClass = malloc(sizeof(CLASS));
    if(NULL == classfile->aClass){
        printf("CP_NO_MEM");
        return CP_NO_MEM;
    }
    res_code = do_parse(classfile,data);
    free(data);
    return res_code;
}


int do_parse(CLASSFILE* classfile,char *data){
    //结果值
    int rescode=CP_OK;
    // 魔数
    classfile->aClass->magic = swap_u4(*(u4*)data);
    // minor_version
    data+=sizeof(u4);
    classfile->aClass->minor_version = swap_u2(*(u2*)data);
    data+=sizeof(u2);
    classfile->aClass->major_version = swap_u2(*(u2*)data);
    data+=sizeof(u2);
    // 常量池大小
    classfile->aClass->constant_pool_count = swap_u2(*(u2*)data);
    // 获取到常量池大小后向前移动两个字节
    data+=sizeof(u2);
    // 分配常量池内存
    if(NULL != classfile->aClass->constant_pool_count)
        classfile->aClass->constant_pool = (CONSTANT_ENTITY*)malloc(sizeof(CONSTANT_ENTITY)*classfile->aClass->constant_pool_count);

    for(u2 i=1;i<classfile->aClass->constant_pool_count;i++)
    {

        u1 element_type = *data;
        // 封装常量池实例
        CONSTANT_ENTITY* constant_entity = malloc(sizeof(CONSTANT_ENTITY));
        constant_entity->type = element_type;
        switch (element_type)
        {
            case CONSTANT_Utf8:
                printf("#%d,CONSTANT_Utf8\n",i);
                CONSTANT_Utf8_info* constant_utf8_info = malloc(sizeof(CONSTANT_Utf8_info));
                constant_utf8_info->tag = element_type;
                data++;
                constant_utf8_info->length = swap_u2(*(u2*)data);
                data+=sizeof(u2);
                constant_utf8_info->bytes = malloc(sizeof(char)*constant_utf8_info->length);
                for(int i = 0; i<constant_utf8_info->length; i++){
                    constant_utf8_info->bytes[i] = *(u1*)data;
                    data++;
                }
                // try to print the bytes
                int the_unicode_count = get_unicode_count(constant_utf8_info->bytes,constant_utf8_info->length);
                unsigned int* unicodes = malloc(the_unicode_count);
                if(unicodes == NULL){
                    printf("cannot malloc memery!!!");
                }
                int the_result = getUnicodes(constant_utf8_info->bytes,constant_utf8_info->length,unicodes);
                if(0 <= the_result){
                    wchar_t wta[the_unicode_count];
                    for(int u = 0; u<the_unicode_count; u++){
                        wta[u] = unicodes[u];
                    }
                    printf("#%d,CONSTANT_Utf8->bytes=>str:",i);
                    if(the_unicode_count>1){
                        wprintf(L"%ls",wta);
                    } else {
                        wprintf( L"%lc", wta[0]);
                    }
                    printf("\n");

                }

                // end print
                constant_entity->vp=constant_utf8_info;
//                data+=constant_utf8_info->length;
                break;
            case CONSTANT_Integer:
                printf("#%d,CONSTANT_Integer\n",i);
                CONSTANT_Integer_info* constant_integer_info = malloc(sizeof(CONSTANT_Integer_info));
                constant_integer_info->tag = element_type;
                data++;
                constant_integer_info->bytes = swap_u4(*(u4*)data);
                data+=sizeof(u4);
                constant_entity->vp = constant_integer_info;
                break;
            case CONSTANT_Float:
                printf("#%d,CONSTANT_Float\n",i);
                CONSTANT_Float_info* constant_float_info = malloc(sizeof(CONSTANT_Float_info));
                constant_float_info->tag = element_type;
                data++;
                constant_float_info->bytes = swap_u4(*(u4*)data);
                data+=sizeof(u4);
                constant_entity->vp = constant_float_info;
                break;
            case CONSTANT_Long:
                printf("#%d,CONSTANT_Long\n",i);
                CONSTANT_Long_info* constant_long_info = malloc(sizeof(CONSTANT_Long_info));
                constant_long_info->tag = element_type;
                data++;
                constant_long_info->high_bytes = swap_u4(*(u4*)data);
                data+=sizeof(u4);
                constant_long_info->low_bytes = swap_u4(*(u4*)data);
                data+=sizeof(u4);
                constant_entity->vp = constant_long_info;
                // Long 和 Double占两个常量池位
                i++;
                break;
            case CONSTANT_Double:
                printf("#%d,CONSTANT_Double\n",i);
                CONSTANT_Double_info* constant_double_info = malloc(sizeof(CONSTANT_Double_info));
                constant_double_info->tag = element_type;
                data++;
                constant_double_info->high_bytes = swap_u4(*(u4*)data);
                data+=sizeof(u4);
                constant_double_info->low_bytes = swap_u4(*(u4*)data);
                data+=sizeof(u4);
                constant_entity->vp = constant_double_info;
                i++;
                break;
            case CONSTANT_Class:
                printf("#%d,CONSTANT_Class\n",i);
                CONSTANT_Class_info* constant_class_info = malloc(sizeof(CONSTANT_Class_info));
                constant_class_info->tag = element_type;
                data++;
                constant_class_info->name_index = swap_u2(*(u2*)data);
                data+= sizeof(u2);
                constant_entity->vp = constant_class_info;
                break;
            case CONSTANT_String:
                printf("#%d,CONSTANT_String\n",i);
                CONSTANT_String_info* constant_string_info = malloc(sizeof(CONSTANT_String_info));
                constant_string_info->tag = element_type;
                data++;
                constant_string_info->string_index = swap_u2(*(u2*)data);
                data+= sizeof(u2);
                constant_entity->vp = constant_string_info;
                break;
            case CONSTANT_Fieldref:
                printf("#%d,CONSTANT_Fieldref\n",i);
                CONSTANT_Fieldref_info* constant_fieldref_info = malloc(sizeof(CONSTANT_Fieldref_info));
                constant_fieldref_info->tag = element_type;
                data++;
                constant_fieldref_info->class_index = swap_u2(*(u2*)data);
                data+= sizeof(u2);
                constant_fieldref_info->name_and_type_index = swap_u2(*(u2*)data);
                data+= sizeof(u2);
                constant_entity->vp = constant_fieldref_info;
                break;
            case CONSTANT_Methodref:
                printf("#%d,CONSTANT_Methodref\n",i);
                CONSTANT_Methodref_info* constant_methodref_info = malloc(sizeof(CONSTANT_Methodref_info));
                constant_methodref_info->tag = element_type;
                data++;
                constant_methodref_info->class_index = swap_u2(*(u2*)data);
                data+= sizeof(u2);
                constant_methodref_info->name_and_type_index = swap_u2(*(u2*)data);
                data+= sizeof(u2);
                constant_entity->vp = constant_methodref_info;
                break;
            case CONSTANT_InterfaceMethodref:
                printf("#%d,CONSTANT_InterfaceMethodref\n",i);
                CONSTANT_InterfaceMethodref_info* constant_interfaceMethodref_info = malloc(sizeof(CONSTANT_InterfaceMethodref_info));
                constant_interfaceMethodref_info->tag = element_type;
                data++;
                constant_interfaceMethodref_info->class_index = swap_u2(*(u2*)data);
                data+= sizeof(u2);
                constant_interfaceMethodref_info->name_and_type_index = swap_u2(*(u2*)data);
                data+= sizeof(u2);
                constant_entity->vp = constant_interfaceMethodref_info;
                break;
            case CONSTANT_NameAndType:
                printf("#%d,CONSTANT_NameAndType\n",i);
                CONSTANT_NameAndType_info* constant_nameAndType_info = malloc(sizeof(CONSTANT_NameAndType_info));
                constant_nameAndType_info->tag = element_type;
                data++;
                constant_nameAndType_info->name_index = swap_u2(*(u2*)data);
                data+= sizeof(u2);
                constant_nameAndType_info->descriptor_index = swap_u2(*(u2*)data);
                data+= sizeof(u2);
                constant_entity->vp = constant_nameAndType_info;
                break;
            case CONSTANT_MethodHandle:
                printf("#%d,CONSTANT_MethodHandle\n",i);
                CONSTANT_MethodHandle_info* constant_methodHandle_info = malloc(sizeof(CONSTANT_MethodHandle_info));
                constant_methodHandle_info->tag = element_type;
                data++;
                constant_methodHandle_info->reference_kind = *(u1*)data;
                data+= sizeof(u1);
                constant_methodHandle_info->reference_index = swap_u2(*(u2*)data);
                data+= sizeof(u2);
                constant_entity->vp = constant_methodHandle_info;
                break;
            case CONSTANT_MethodType:
                printf("#%d,CONSTANT_MethodType\n",i);
                CONSTANT_MethodType_info* constant_methodType_info = malloc(sizeof(CONSTANT_MethodType_info));
                constant_methodType_info->tag = element_type;
                data++;
                constant_methodType_info->descriptor_index = swap_u2(*(u2*)data);
                data+= sizeof(u2);
                constant_entity->vp = constant_methodType_info;
                break;
            case CONSTANT_InvokeDynamic:
                printf("#%d,CONSTANT_InvokeDynamic\n",i);
                CONSTANT_InvokeDynamic_info* constant_invokeDynamic_info = malloc(sizeof(CONSTANT_InvokeDynamic_info));
                constant_invokeDynamic_info->tag = element_type;
                data++;
                constant_invokeDynamic_info->bootstrap_method_attr_index = swap_u2(*(u2*)data);
                data+= sizeof(u2);
                constant_invokeDynamic_info->name_and_type_index = swap_u2(*(u2*)data);
                data+= sizeof(u2);
                constant_entity->vp = constant_invokeDynamic_info;
                break;
            case CONSTANT_Module:
                printf("#%d,CONSTANT_Module\n",i);
                break;
            case CONSTANT_Package:
                printf("#%d,CONSTANT_Package\n",i);
                break;
            default:
                printf("#%d,WRONG CONSTANT_CODE! element_type = %d\n",i,element_type);
                return CP_PARSE_CONSTANTS_ERROR;
        }
        classfile->aClass->constant_pool[i] = *constant_entity;

    }

    classfile->aClass->access_flags = swap_u2(*(u2*)data);
    data+=sizeof(u2);
    classfile->aClass->this_class = swap_u2(*(u2*)data);
    data+=sizeof(u2);
    classfile->aClass->super_class = swap_u2(*(u2*)data);
    data+=sizeof(u2);
    classfile->aClass->interfaces_count = swap_u2(*(u2*)data);
    data+=sizeof(u2);
    if(classfile->aClass->interfaces_count > 0){
        classfile->aClass->interfaces = malloc(sizeof(u2)*classfile->aClass->interfaces_count);
        for(int f = 0; f < classfile->aClass->interfaces_count; f++){
            classfile->aClass->interfaces[f] = swap_u2(*(u2*)data);
            data+=sizeof(u2);
        }
    }
    classfile->aClass->fields_count = swap_u2(*(u2*)data);
    data+=sizeof(u2);

    // 解析字段
    if(classfile->aClass->fields_count > 0){
        classfile->aClass->fields = malloc(sizeof(field_info)*classfile->aClass->fields_count);
        for(int i=0; i<classfile->aClass->fields_count; i++){
            classfile->aClass->fields[i].access_flags = swap_u2(*(u2*)data);
            data+=sizeof(u2);
            classfile->aClass->fields[i].name_index = swap_u2(*(u2*)data);
            data+=sizeof(u2);
            classfile->aClass->fields[i].descriptor_index = swap_u2(*(u2*)data);
            data+=sizeof(u2);
            classfile->aClass->fields[i].attributes_count = swap_u2(*(u2*)data);
            data+=sizeof(u2);
            if(classfile->aClass->fields[i].attributes_count > 0){
                classfile->aClass->fields[i].attributes =
                        malloc(sizeof(attribute_info)*classfile->aClass->fields[i].attributes_count);
                for(int m=0; m<classfile->aClass->fields[i].attributes_count;m++){
                    classfile->aClass->fields[i].attributes[m].attribute_name_index = swap_u2(*(u2*)data);
                    data+=sizeof(u2);
                    classfile->aClass->fields[i].attributes[m].attribute_length = swap_u4(*(u4*)data);
                    data+= sizeof(u4);
                    if(classfile->aClass->fields[i].attributes[m].attribute_length > 0){
                        classfile->aClass->fields[i].attributes[m].info =
                                malloc(sizeof(u1)*classfile->aClass->fields[i].attributes[m].attribute_length);
                        for(int n = 0; n<classfile->aClass->fields[i].attributes[m].attribute_length;n++){
                            classfile->aClass->fields[i].attributes[m].info[n] = *(u1*)data;
                            data+= sizeof(u1);
                        }
                    }
                }
            }
        }
    }

    classfile->aClass->methods_count = swap_u2(*(u2*)data);
    data+=sizeof(u2);

    // parse methods
    if(classfile->aClass->methods_count > 0){
        classfile->aClass->methods = malloc(sizeof(method_info)*classfile->aClass->methods_count);
        for(int i=0; i<classfile->aClass->methods_count; i++){
            classfile->aClass->methods[i].access_flags = swap_u2(*(u2*)data);
            data+=sizeof(u2);
            classfile->aClass->methods[i].name_index = swap_u2(*(u2*)data);
            data+=sizeof(u2);
            classfile->aClass->methods[i].descriptor_index = swap_u2(*(u2*)data);
            data+=sizeof(u2);
            classfile->aClass->methods[i].attributes_count = swap_u2(*(u2*)data);
            data+=sizeof(u2);
            if(classfile->aClass->methods[i].attributes_count > 0){
                classfile->aClass->methods[i].attributes =
                        malloc(sizeof(attribute_info)*classfile->aClass->methods[i].attributes_count);
                for(int m=0; m<classfile->aClass->methods[i].attributes_count;m++){
                    classfile->aClass->methods[i].attributes[m].attribute_name_index = swap_u2(*(u2*)data);
                    data+=sizeof(u2);
                    classfile->aClass->methods[i].attributes[m].attribute_length = swap_u4(*(u4*)data);
                    data+= sizeof(u4);
                    if(classfile->aClass->methods[i].attributes[m].attribute_length > 0){
                        classfile->aClass->methods[i].attributes[m].info =
                                malloc(sizeof(u1)*classfile->aClass->methods[i].attributes[m].attribute_length);
                        for(int n = 0; n<classfile->aClass->methods[i].attributes[m].attribute_length;n++){
                            classfile->aClass->methods[i].attributes[m].info[n] = *(u1*)data;
                            data+= sizeof(u1);
                        }
                    }
                }
            }
        }
    }

    classfile->aClass->attributes_count = swap_u2(*(u2*)data);
    data+=sizeof(u2);
    // parse attributes
    if(classfile->aClass->attributes_count > 0){
        classfile->aClass->attributes = malloc(sizeof(attribute_info)*classfile->aClass->attributes_count);
        for(int m=0; m<classfile->aClass->attributes_count;m++){
            classfile->aClass->attributes->attribute_name_index = swap_u2(*(u2*)data);
            data+=sizeof(u2);
            classfile->aClass->attributes->attribute_length = swap_u4(*(u4*)data);
            data+= sizeof(u4);
            if(classfile->aClass->attributes->attribute_length > 0){
                classfile->aClass->attributes->info =
                        malloc(sizeof(u1)*classfile->aClass->attributes->attribute_length);
                for(int n = 0; n<classfile->aClass->attributes->attribute_length;n++){
                    classfile->aClass->attributes->info[n] = *(u1*)data;
                    data+= sizeof(u1);
                }
            }
        }
    }


    return rescode;
}