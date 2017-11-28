#include <stdio.h>

#include "endian_swap/endian_swap.h"
#include "class_files/constant/class_constants.h"


int main(){
    char* path = "/Users/shangyanshuo/Documents/generation/2017/codes/javaDemos/Test.class";
    CLASSFILE* classfile;
    if((classfile = class_open(path)) == NULL){
        printf("error when open file!");
        return -1;
    }
    int res_code = class_parse(classfile);
    printf("the res_code=%d\n",res_code);
    if(CP_OK == res_code){
        printf("magic=%x\n",classfile->aClass->magic);
        printf("minor_version=%x\n",classfile->aClass->minor_version);
        printf("major_version=%x\n",classfile->aClass->major_version);
        printf("constant_pool_count=%d\n",classfile->aClass->constant_pool_count);

        printf("access_flags=%x\n",classfile->aClass->access_flags);
        printf("this_class=%d\n",classfile->aClass->this_class);
        printf("super_class=%d\n",classfile->aClass->super_class);
        printf("interfaces_count=%d\n",classfile->aClass->interfaces_count);
        if(classfile->aClass->interfaces_count>0){
            for(int f = 0; f < classfile->aClass->interfaces_count; f++){
                printf("interfaces[%d]=%d\n",f,classfile->aClass->interfaces[f]);
            }
        }
        printf("fields_count=%d\n",classfile->aClass->fields_count);

        printf("methods_count=%d\n",classfile->aClass->methods_count);

        printf("attributes_count=%d\n",classfile->aClass->attributes_count);
        return 0;
        // 打印常量池
        if(classfile->aClass->constant_pool_count)
        {
            for(u2 i=1;i<classfile->aClass->constant_pool_count;i++)
            {
                switch (classfile->aClass->constant_pool[i].type)
                {
                    case CONSTANT_Utf8:
                        printf("--------#%d CONSTANT_Utf8--------",i);
                        CONSTANT_Utf8_info* constant_utf8_info = (CONSTANT_Utf8_info*)classfile->aClass->constant_pool[i].vp;
                        printf("CONSTANT_Utf8->tag:\t\t\t%d\n",constant_utf8_info->tag);
                        printf("CONSTANT_Utf8->length:\t\t\t%d\n",constant_utf8_info->length);
                        printf("CONSTANT_Utf8->bytes:\t\t\t%s\n",constant_utf8_info->bytes);
                        break;
                    case CONSTANT_Integer:
                        printf("CONSTANT_Integer");
                        CONSTANT_Integer_info* constant_integer_info = (CONSTANT_Integer_info*)classfile->aClass->constant_pool[i].vp;
                        printf("CONSTANT_Integer->tag:\t\t\t%d\n",constant_integer_info->tag);
                        printf("CONSTANT_Integer->bytes:\t\t\t%llx\n",constant_integer_info->bytes);
                        break;
                    case CONSTANT_Float:
                        printf("CONSTANT_Float");
                        CONSTANT_Float_info* constant_float_info = (CONSTANT_Float_info*)classfile->aClass->constant_pool[i].vp;
                        printf("CONSTANT_Float->tag:\t\t\t%d\n",constant_float_info->tag);
                        // 这里需要斟酌一下
                        printf("CONSTANT_Float->tag:\t\t\t%llx\n",constant_float_info->bytes);
                        break;
                    case CONSTANT_Long:
                        printf("CONSTANT_Long");
                        // 该类型需要查看JAVA文档确认
                        CONSTANT_Long_info* constant_long_info = (CONSTANT_Long_info*)classfile->aClass->constant_pool[i].vp;

                        break;
                    case CONSTANT_Double:
                        printf("CONSTANT_Double");
                        CONSTANT_Double_info* constant_double_info = (CONSTANT_Double_info*)classfile->aClass->constant_pool[i].vp;

                        break;
                    case CONSTANT_Class:
                        printf("CONSTANT_Class");
                        CONSTANT_Class_info* constant_class_info = (CONSTANT_Class_info*)classfile->aClass->constant_pool[i].vp;

                        break;
                    case CONSTANT_String:
                        printf("CONSTANT_String");
                        CONSTANT_String_info* constant_string_info = (CONSTANT_String_info*)classfile->aClass->constant_pool[i].vp;

                        break;
                    case CONSTANT_Fieldref:
                        printf("CONSTANT_Fieldref");
                        CONSTANT_Fieldref_info* constant_fieldref_info = (CONSTANT_Fieldref_info*)classfile->aClass->constant_pool[i].vp;

                        break;
                    case CONSTANT_Methodref:
                        printf("CONSTANT_Methodref");
                        CONSTANT_Methodref_info* constant_methodref_info = (CONSTANT_Methodref_info*)classfile->aClass->constant_pool[i].vp;

                        break;
                    case CONSTANT_InterfaceMethodref:
                        printf("CONSTANT_InterfaceMethodref");
                        CONSTANT_InterfaceMethodref_info* constant_interfaceMethodref_info = (CONSTANT_InterfaceMethodref_info*)classfile->aClass->constant_pool[i].vp;

                        break;
                    case CONSTANT_NameAndType:
                        printf("CONSTANT_NameAndType");
                        CONSTANT_NameAndType_info* constant_nameAndType_info = (CONSTANT_NameAndType_info*)classfile->aClass->constant_pool[i].vp;

                        break;
                    case CONSTANT_MethodHandle:
                        printf("CONSTANT_MethodHandle");
                        CONSTANT_MethodHandle_info* constant_methodHandle_info = (CONSTANT_MethodHandle_info*)classfile->aClass->constant_pool[i].vp;

                        break;
                    case CONSTANT_MethodType:
                        printf("CONSTANT_MethodType");
                        CONSTANT_MethodType_info* constant_methodType_info = (CONSTANT_MethodType_info*)classfile->aClass->constant_pool[i].vp;

                        break;
                    case CONSTANT_InvokeDynamic:
                        printf("CONSTANT_InvokeDynamic");
                        CONSTANT_InvokeDynamic_info* constant_invokeDynamic_info = (CONSTANT_InvokeDynamic_info*)classfile->aClass->constant_pool[i].vp;

                        break;
                    case CONSTANT_Module:
                        printf("CONSTANT_Module");
                        break;
                    case CONSTANT_Package:
                        printf("CONSTANT_Package");
                        break;
                    default:
                        printf("WRONG CONSTANT_CODE!");
                        return CP_PARSE_CONSTANTS_ERROR;
                }
            }
        }

    } else
        printf("error");

}
