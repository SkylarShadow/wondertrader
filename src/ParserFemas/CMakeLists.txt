
cmake_minimum_required(VERSION 3.10)


project(ParserFemas LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(SRC  
	${PROJECT_SOURCE_DIR}/ParserFemas.cpp
	${PROJECT_SOURCE_DIR}/ParserFemas.h
)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin)

include_directories(${INCS})
link_directories(${LNKS})
add_library(ParserFemas SHARED ${SRC})

if(MSVC)
else()
	set(LIBS
		boost_filesystem
		dl
	)
	target_link_libraries(ParserFemas PRIVATE ${LIBS})
endif()

if(MSVC)
else()
	set_target_properties(ParserFemas PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		C_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif()

