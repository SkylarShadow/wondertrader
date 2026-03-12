
cmake_minimum_required(VERSION 3.10)

#2. 确定工程名
project(TraderOES LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(SRC  
	${PROJECT_SOURCE_DIR}/TraderOES.cpp
	${PROJECT_SOURCE_DIR}/TraderOES.h	
)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin)

include_directories(${INCS})
link_directories(${LNKS})
add_library(TraderOES SHARED ${SRC})

if(MSVC)
	set(LIBS
		ws2_32
		${PROJECT_SOURCE_DIR}/../API/oesApi0.17.5.8/${PLATFORM}/oes_api.lib
		)
else(GNUCC)
	if(WIN32)
		set(LIBS
			dl
			boost_thread
			boost_filesystem
			ws2_32
			${PROJECT_SOURCE_DIR}/../API/oesApi0.17.5.8/${PLATFORM}/oes_api.lib
			)
	else(UNIX)
		set(LIBS
			dl
			boost_thread
			boost_filesystem
			${PROJECT_SOURCE_DIR}/../API/oesApi0.17.5.8/linux/liboes_api.a
			)
	endif()
endif()
target_link_libraries(TraderOES PRIVATE ${LIBS})

if (MSVC)
else (GNUCC)
	set_target_properties(TraderOES PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		C_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif ()

