cmake_minimum_required(VERSION 2.8)
project(test)

set(CMAKE_BUILD_TYPE "Debug")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive -mavx2 -Wa,-q -std=c++11  -O2")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -O2  -Wall -m64 -mavx2")

include_directories(./include)
aux_source_directory(./src DIR_SRCS)
# aux_source_directory(./test DIR_SRCS)

add_executable(demo ./test/AVX_SM4_TEST.c ${DIR_SRCS})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)