
cmake_minimum_required(VERSION 3.10)


project(ParserShm LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(SRC  
	${PROJECT_SOURCE_DIR}/ParserShm.cpp
	${PROJECT_SOURCE_DIR}/ParserShm.h
)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin)

include_directories(${INCS})
link_directories(${LNKS})
add_library(ParserShm SHARED ${SRC})

if(MSVC)
	set(LIBS )
else()
	if (WIN32)
		set(LIBS
			boost_thread
            boost_filesystem
		)
	else(UNIX)
		set(LIBS
			boost_thread
            boost_filesystem
		)
	endif()
endif()
target_link_libraries(ParserShm PRIVATE ${LIBS})

if(MSVC)
else()
	set_target_properties(ParserShm PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		C_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif()

