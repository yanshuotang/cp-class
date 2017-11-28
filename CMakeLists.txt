cmake_minimum_required(VERSION 3.8)
project(cp-class)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES endian_swap/endian_swap.h endian_swap/endian_swap.c main.c cpclass.h cpclass.c class_files/constant/class_constants.h class_files/constant/class_constants.c common/common_type.h utils/utf_8_tools/utf_8_tools.h utils/utf_8_tools/utf_8_tools.c)
add_executable(cp_class ${SOURCE_FILES})