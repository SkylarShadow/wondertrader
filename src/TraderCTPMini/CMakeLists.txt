cmake_minimum_required(VERSION 3.10)


project(TraderCTPMini LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(SRC  
	${PROJECT_SOURCE_DIR}/TraderCTPMini.cpp
	${PROJECT_SOURCE_DIR}/TraderCTPMini.h
)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin)

include_directories(${INCS})
link_directories(${LNKS})
add_library(TraderCTPMini SHARED ${SRC})

if(MSVC)
else()
	set(LIBS
		pthread
		boost_filesystem
		dl
		)
	target_link_libraries(TraderCTPMini PRIVATE ${LIBS})
endif()

if(MSVC)
else()
	set_target_properties(TraderCTPMini PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		C_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif()

