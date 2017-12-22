cmake_minimum_required(VERSION 3.5)
project(cp-class)

set(CMAKE_C_STANDARD 99)

#Appends the cmake/modules path inside the MAKE_MODULE_PATH variable which stores the
# directories of additional CMake modules
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

ADD_SUBDIRECTORY(src)

