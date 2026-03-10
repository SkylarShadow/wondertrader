
cmake_minimum_required(VERSION 3.10)


project(ZtDtServo LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/ZtDtPorter)

#源码
file(GLOB SRCS *.cpp)
file(GLOB HDRS *.h)

if(MSVC)
    list (APPEND SRCS ../Common/mdump.cpp)
endif()

include_directories(${INCS})
link_directories(${LNKS})
add_library(ZtDtServo SHARED ${SRCS} ${HDRS})

set(LIBS
    ZTSTools
    ZTSUtils
)
if(MSVC)
else()
    list(APPEND LIBS
        dl
        boost_filesystem
        )
    if(WIN32)
        list(APPEND LIBS iconv)
    endif()
endif()

target_link_libraries(ZtDtServo PRIVATE ${LIBS})
if(MSVC)
else()
    set_target_properties(ZtDtServo PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif()