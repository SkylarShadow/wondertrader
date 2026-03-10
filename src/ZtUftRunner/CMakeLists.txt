cmake_minimum_required(VERSION 3.10)


project(ZtUftRunner LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/ZtUftRunner)

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

add_executable(ZtUftRunner ${SRCS} ${HDRS})
target_link_libraries(ZtUftRunner PRIVATE ${LIBS})

if(MSVC)
else()
	set_target_properties(ZtUftRunner PROPERTIES
        LINK_FLAGS_RELEASE -s)
endif()

file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/parsers/)
file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/traders/)



add_custom_command(TARGET ZtUftRunner POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E
	copy ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/${PREFIX}TraderCTP${SUFFIX} ${EXECUTABLE_OUTPUT_PATH}/traders/
    
    COMMAND ${CMAKE_COMMAND} -E
	copy ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/${PREFIX}ParserCTP${SUFFIX} ${EXECUTABLE_OUTPUT_PATH}/parsers/

	COMMENT "depends modules copied")