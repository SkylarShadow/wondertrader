cmake_minimum_required(VERSION 3.10)


project(ZtDataStorage LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin)

#源码
file(GLOB SRCS *.cpp)
file(GLOB HDRS *.h)

include_directories(${INCS})
link_directories(${LNKS})
add_library(ZtDataStorage SHARED ${SRCS} ${HDRS})

set(LIBS
	ZTSUtils
)

if(MSVC)
else()
	list(APPEND LIBS
		dl
		pthread
		boost_filesystem
		)
	if(WIN32)
		list(APPEND LIBS iconv)
	endif()
endif()
target_link_libraries(ZtDataStorage PRIVATE ${LIBS})

if(MSVC)
else()
	set_target_properties(ZtDataStorage PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif()

