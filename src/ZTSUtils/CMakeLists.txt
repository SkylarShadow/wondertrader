
#1. 确定CMake的最低版本需求
cmake_minimum_required(VERSION 3.0.0)


project(ZTSUtils LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

if(MSVC)
else()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/libs)

list(APPEND INCS "./yamlcpp")

#源码
file(GLOB SRCS "*.cpp" "./zstdlib/*.c" "./yamlcpp/*.cpp" "./lmdb/*.c")
file(GLOB HDRS "*.h" "./zstdlib/*.h")

if(MSVC)
list(APPEND SRCS "./StackTracer/StackTracer.cpp" "./StackTracer/StackWalker.cpp")
else()
list(APPEND SRCS "./StackTracer/StackTracer.cpp")
endif()

include_directories(${INCS})

# 编译成动态库，并生成库文件
add_library(ZTSUtils STATIC ${SRCS} ${HDRS})