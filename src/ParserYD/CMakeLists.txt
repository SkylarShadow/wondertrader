cmake_minimum_required(VERSION 3.10)

#2. 确定工程名
project(ParserYD LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(SRC  
	${PROJECT_SOURCE_DIR}/ParserYD.cpp
	${PROJECT_SOURCE_DIR}/ParserYD.h
)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin)

include_directories(${INCS})
link_directories(${LNKS})
add_library(ParserYD SHARED ${SRC})

if (MSVC)
else (GNUCC)
	set(LIBS
		boost_thread
		boost_filesystem
		dl
	)
	target_link_libraries(ParserYD PRIVATE ${LIBS})
endif()

if (MSVC)
else (GNUCC)
	set_target_properties(ParserYD PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		C_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif ()

