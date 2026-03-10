
#1. 确定CMake的最低版本需求
cmake_minimum_required(VERSION 3.10)

project(ZtBtCore LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

if(MSVC)
else()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/libs)

#源码
file(GLOB SRCS *.cpp)
file(GLOB HDRS *.h)

include_directories(${INCS})
link_directories(${LNKS})

add_library(ZtBtCore STATIC ${SRCS} ${HDRS})