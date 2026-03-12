
cmake_minimum_required(VERSION 3.10)

#2. 确定工程名
project(TraderATP LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(SRC  
	${PROJECT_SOURCE_DIR}/TraderATP.cpp
	${PROJECT_SOURCE_DIR}/TraderATP.h	
)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin)

include_directories(${INCS})
link_directories(${LNKS})
add_library(TraderATP SHARED ${SRC})

if(MSVC)
	set(LIBS
		ws2_32
		${PROJECT_SOURCE_DIR}/../API/AtpTradeApi/${PLATFORM}/atptradeapi.lib
		)
elseif(GNUCC)
	if(WIN32)
		set(LIBS
			dl
			boost_thread
			boost_filesystem
			ws2_32
			${PROJECT_SOURCE_DIR}/../API/AtpTradeApi/${PLATFORM}/atptradeapi.lib
			)
	elseif(UNIX)
		set(LIBS
			dl
			boost_thread
			boost_filesystem
			# 似乎暂时没有Linux版本的库
			)
	endif()
endif()
target_link_libraries(TraderATP PRIVATE ${LIBS})

if (MSVC)
elseif (GNUCC)
	set_target_properties(TraderATP PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		C_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif ()

