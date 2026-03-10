
cmake_minimum_required(VERSION 3.10)

#2. 确定工程名
project(ParserOES LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(SRC  
	${PROJECT_SOURCE_DIR}/ParserOES.cpp
	${PROJECT_SOURCE_DIR}/ParserOES.h
)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin)

include_directories(${INCS})
link_directories(${LNKS})
add_library(ParserOES SHARED ${SRC})

if(MSVC)
	set(LIBS
		ws2_32
	)
elseif(GNUCC)
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
target_link_libraries(ParserOES PRIVATE ${LIBS})

if (MSVC)
else (GNUCC)
	set_target_properties(ParserOES PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		C_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif()

