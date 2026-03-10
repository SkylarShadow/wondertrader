
cmake_minimum_required(VERSION 3.10)


project(TraderMocker LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(SRC  
	${PROJECT_SOURCE_DIR}/TraderMocker.cpp
	${PROJECT_SOURCE_DIR}/TraderMocker.h
)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin)

include_directories(${INCS})
link_directories(${LNKS})
add_library(TraderMocker SHARED ${SRC})

if(MSVC)
	set(LIBS
		ws2_32
		)
else()
	if(WIN32)
		set(LIBS
			boost_thread
			boost_filesystem
			ws2_32
		)
	else(UNIX)
		set(LIBS
			boost_thread
			boost_filesystem
		)
	endif()
endif()
target_link_libraries(TraderMocker PRIVATE ${LIBS})

if(MSVC)
else()
	set_target_properties(TraderMocker PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		C_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif()

