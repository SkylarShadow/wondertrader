
cmake_minimum_required(VERSION 3.10)


project(ZtTester LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/ZtBtRunner)

include_directories(${INCS})
link_directories(${LNKS})

set(LIBS
	ZtBtCore
	ZTSTools
	ZTSUtils
)

if(MSVC)
else()
	list(APPEND LIBS
		boost_filesystem
		pthread
		dl)
	if(WIN32)
		list(APPEND LIBS iconv)
	endif()
endif()

#添加源码
file(GLOB SRCS *.cpp)
file(GLOB HDRS *.h)

if(MSVC)
	list (APPEND SRCS ../Common/mdump.cpp)
endif()

#可执行文件
add_executable(ZtBtRunner ${SRCS} ${HDRS})
target_link_libraries(ZtBtRunner PRIVATE ${LIBS})

if(MSVC)
else()
	set_target_properties(ZtBtRunner PROPERTIES
        LINK_FLAGS_RELEASE -s)
endif()

#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/ZtBtPorter)
#add_executable(TestBtPorter ../TestBtPorter/TestBtPorter.cpp)
#target_link_libraries(TestBtPorter ${LIBS})

