
cmake_minimum_required(VERSION 3.10)


project(TestTrader LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/TestTrader)

#源码
file(GLOB SRCS *.cpp)

set(LIBS
	ZTSTools
	ZTSUtils
	)
    
if(MSVC)
else()
	list(APPEND LIBS
		dl
        pthread
		boost_filesystem)
    if(WIN32)
		list(APPEND LIBS
			iconv)
	endif()
endif()

include_directories(${INCS})
link_directories(${LNKS})

add_executable(TestTrader ${SRCS})
target_link_libraries(TestTrader PRIVATE ${LIBS})

if(MSVC)
else()
	set_target_properties(TestTrader PROPERTIES
        LINK_FLAGS_RELEASE -s)
endif()