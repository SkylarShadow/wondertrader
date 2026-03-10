cmake_minimum_required(VERSION 3.10)


project(ZtMsgQue LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin)

#源码
file(GLOB SRCS *.cpp)
file(GLOB HDRS *.h)

include_directories(${INCS})
link_directories(${LNKS})
add_library(ZtMsgQue SHARED ${SRCS} ${HDRS})

if(MSVC)
	set(LIBS
		nanomsg
		ws2_32
		mswsock
		)
else()
	if(WIN32)
		set(LIBS
			dl
			pthread
			nanomsg
			ws2_32
			mswsock
		)
	else(UNIX)
		set(LIBS
			dl
			pthread
			nanomsg
		)
	endif()
endif()

target_link_libraries(ZtMsgQue PRIVATE ${LIBS})
if(MSVC)
else()
	set_target_properties(ZtMsgQue PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif()