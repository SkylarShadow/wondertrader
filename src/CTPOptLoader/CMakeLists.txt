cmake_minimum_required(VERSION 3.10)

project(CTPOptLoader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
# bin输出目录
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/Loader)

#源码
set(SRC  
	${PROJECT_SOURCE_DIR}/CTPOptLoader.cpp
	${PROJECT_SOURCE_DIR}/TraderSpi.cpp
	${PROJECT_SOURCE_DIR}/TraderSpi.h
)

set(LIBS	
	# boost_filesystem
    ZTSUtils
)

add_library(CTPOptLoader SHARED ${SRC})
include_directories(${INCS})
link_directories(${LNKS})



# 链接，通用依赖
target_link_libraries(CTPOptLoader
    PRIVATE
        ${LIBS}
)

# 平台相关依赖
if(MSVC)
else()
	if(WIN32)
	    target_link_libraries(CTPOptLoader
            PRIVATE
                dl
        )
    endif()
    target_link_libraries(CTPOptLoader
        PRIVATE
            iconv
    )
endif()


if(MSVC)
else()
	set_target_properties(CTPOptLoader PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		C_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif()