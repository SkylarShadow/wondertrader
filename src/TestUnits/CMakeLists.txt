
cmake_minimum_required(VERSION 3.10)


project(TestUnits LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/TestUnits)

#源码
file(GLOB SRCS *.cpp ./gtest/*.cc)

set(LIBS
    ZTSTools
	ZTSUtils
    ZtShareHelper)
if(MSVC)
else()
    list(APPEND LIBS pthread boost_filesystem)
	if(WIN32)
		list(APPEND LIBS iconv)
	endif()
endif()

list(APPEND INCS "./gtest")

include_directories(${INCS})
link_directories(${LNKS})

add_executable(TestUnits ${SRCS})
target_link_libraries(TestUnits PRIVATE ${LIBS})

if(MSVC)
else()
	set_target_properties(TestUnits PROPERTIES
        LINK_FLAGS_RELEASE -s)
endif()