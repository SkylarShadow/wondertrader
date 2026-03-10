cmake_minimum_required(VERSION 3.10)


project(ZtDtPorter LANGUAGES CXX)
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
add_library(ZtDtPorter SHARED ${SRCS} ${HDRS})

set(LIBS
	ZtDtCore
	ZTSTools
	ZTSUtils
	)
if(MSVC)
else()
	list(APPEND LIBS
		dl
		pthread
		boost_filesystem
		boost_thread
		)
	if(WIN32)
		list(APPEND LIBS
			ws2_32 iconv
		)
	endif()
endif()

target_link_libraries(ZtDtPorter PRIVATE ${LIBS})
if(MSVC)
else()
	set_target_properties(ZtDtPorter PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif()

file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH}/parsers/)
add_custom_command(TARGET ZtDtPorter POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E
		copy ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/${PREFIX}ParserCTP${SUFFIX} ${LIBRARY_OUTPUT_PATH}/parsers/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/${PREFIX}ParserXTP${SUFFIX} ${LIBRARY_OUTPUT_PATH}/parsers/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/${PREFIX}ParserFemas${SUFFIX} ${LIBRARY_OUTPUT_PATH}/parsers/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/${PREFIX}ParserCTPMini${SUFFIX} ${LIBRARY_OUTPUT_PATH}/parsers/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/${PREFIX}ZtDataStorage${SUFFIX} ${LIBRARY_OUTPUT_PATH}/

		COMMAND ${CMAKE_COMMAND} -E
		copy ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/${PREFIX}ZtDataStorageAD${SUFFIX} ${LIBRARY_OUTPUT_PATH}/

		COMMENT "depends modules copied")
		