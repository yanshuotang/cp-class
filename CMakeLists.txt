cmake_minimum_required(VERSION 3.5)
project(cp-class)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES src/endian_swap/endian_swap.h src/endian_swap/endian_swap.c src/main.c src/cpclass.h src/cpclass.c src/class_files/constant/class_constants.h src/class_files/constant/class_constants.c src/common/common_type.h src/utils/utf_8_tools/utf_8_tools.h src/utils/utf_8_tools/utf_8_tools.c)
add_executable(cp_class ${SOURCE_FILES})
