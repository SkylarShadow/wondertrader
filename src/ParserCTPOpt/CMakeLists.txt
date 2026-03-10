
cmake_minimum_required(VERSION 3.10)


project(ParserCTPOpt LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(SRC  
	${PROJECT_SOURCE_DIR}/ParserCTPOpt.cpp
	${PROJECT_SOURCE_DIR}/ParserCTPOpt.h
)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin)

add_library(ParserCTPOpt SHARED ${SRC})
include_directories(${INCS})
link_directories(${LNKS})


if(MSVC)
else()
	set(LIBS
		boost_thread
		boost_filesystem
		dl
	)
	target_link_libraries(ParserCTPOpt PRIVATE ${LIBS})
endif()

if(MSVC)
else()
	set_target_properties(ParserCTPOpt PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		C_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif()

