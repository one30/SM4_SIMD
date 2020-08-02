cmake_minimum_required(VERSION 2.8)
project(test)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive -mavx2 -Wa,-q -std=c++11  -O3")
# set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -O2  -Wall -m64")

include_directories(./include)
aux_source_directory(./src DIR_SRCS)
# aux_source_directory(./test DIR_SRCS)

add_executable(test ./test/AVX_SM4_TEST.cpp ${DIR_SRCS})
