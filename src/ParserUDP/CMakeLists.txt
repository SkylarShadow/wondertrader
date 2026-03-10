
cmake_minimum_required(VERSION 3.10)


project(ParserUDP LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(SRC  
	${PROJECT_SOURCE_DIR}/ParserUDP.cpp
	${PROJECT_SOURCE_DIR}/ParserUDP.h
)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin)

include_directories(${INCS})
link_directories(${LNKS})
add_library(ParserUDP SHARED ${SRC})

if(MSVC)
	set(LIBS
		ws2_32
	)
else()
	if (WIN32)
		set(LIBS
			boost_thread
			ws2_32
		)
	else(UNIX)
		set(LIBS
			boost_thread
		)
	endif()
endif()
target_link_libraries(ParserUDP ${LIBS})

if(MSVC)
else()
	set_target_properties(ParserUDP PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		C_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif()

