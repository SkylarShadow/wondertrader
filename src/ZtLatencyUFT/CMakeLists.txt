cmake_minimum_required(VERSION 3.10)


project(ZtLatencyUFT LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/ZtLatencyUFT)

#源码
file(GLOB SRCS *.cpp)
file(GLOB HDRS *.h)

if(MSVC)
	list (APPEND SRCS ../Common/mdump.cpp)
endif()

include_directories(${INCS})
link_directories(${LNKS})

set(LIBS
	ZtUftCore
	ZTSTools
    ZTSUtils
    ZtShareHelper
	)

if(MSVC)
	list(APPEND LIBS ws2_32)
else()
	list(APPEND LIBS
		dl
		pthread
		boost_filesystem
		boost_thread)
	if(WIN32)
		list(APPEND LIBS
			ws2_32 iconv)
	endif()
endif()

add_executable(ZtLatencyUFT ${SRCS} ${HDRS})
target_link_libraries(ZtLatencyUFT PRIVATE ${LIBS})

if(MSVC)
else()
	set_target_properties(ZtLatencyUFT PROPERTIES
        LINK_FLAGS_RELEASE -s)
endif()
