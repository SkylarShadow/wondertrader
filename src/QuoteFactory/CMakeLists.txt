cmake_minimum_required(VERSION 3.10)


project(QuoteFactory LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/QuoteFactory)

#源码
file( GLOB SRCS *.cpp)

if(MSVC)
	list (APPEND SRCS ../Common/mdump.cpp)
endif()

link_directories(${LNKS})

# 依赖
set(LIBS
	ZTSTools
	ZTSUtils
	ZtDtCore)

if(MSVC)
	list(APPEND LIBS ws2_32)
else()
	list(APPEND LIBS
		dl
		boost_filesystem
        boost_thread
        pthread
		)
	if(WIN32)
		list(APPEND LIBS
			ws2_32 iconv)
	endif()
endif()



#可执行文件
add_executable(QuoteFactory ${SRCS})
include_directories(${INCS})
target_link_libraries(QuoteFactory ${LIBS})



if(MSVC)
else()
	set_target_properties(QuoteFactory PROPERTIES
        LINK_FLAGS_RELEASE -s)
endif()

file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/parsers/)

if(MSVC)
add_custom_command(TARGET QuoteFactory POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E
	copy ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/$<CONFIG>/${PREFIX}ParserCTP${SUFFIX} ${EXECUTABLE_OUTPUT_PATH}/parsers/

	COMMAND ${CMAKE_COMMAND} -E
	copy ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/$<CONFIG>/${PREFIX}ParserXTP${SUFFIX} ${EXECUTABLE_OUTPUT_PATH}/parsers/

	COMMAND ${CMAKE_COMMAND} -E
	copy ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/$<CONFIG>/${PREFIX}ParserFemas${SUFFIX} ${EXECUTABLE_OUTPUT_PATH}/parsers/

	COMMAND ${CMAKE_COMMAND} -E
	copy ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/$<CONFIG>/${PREFIX}ParserCTPMini${SUFFIX} ${EXECUTABLE_OUTPUT_PATH}/parsers/

	COMMAND ${CMAKE_COMMAND} -E
	copy ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/$<CONFIG>/${PREFIX}ZtDataStorage${SUFFIX} ${EXECUTABLE_OUTPUT_PATH}/

	COMMENT "depends modules copied")

else()
add_custom_command(TARGET QuoteFactory POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E
	copy ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/${PREFIX}ParserCTP${SUFFIX} ${EXECUTABLE_OUTPUT_PATH}/parsers/

	COMMAND ${CMAKE_COMMAND} -E
	copy ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/${PREFIX}ParserXTP${SUFFIX} ${EXECUTABLE_OUTPUT_PATH}/parsers/

	COMMAND ${CMAKE_COMMAND} -E
	copy ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/${PREFIX}ParserFemas${SUFFIX} ${EXECUTABLE_OUTPUT_PATH}/parsers/

	COMMAND ${CMAKE_COMMAND} -E
	copy ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/${PREFIX}ParserCTPMini${SUFFIX} ${EXECUTABLE_OUTPUT_PATH}/parsers/

	COMMAND ${CMAKE_COMMAND} -E
	copy ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/${PREFIX}ZtDataStorage${SUFFIX} ${EXECUTABLE_OUTPUT_PATH}/

	COMMENT "depends modules copied")
endif()

