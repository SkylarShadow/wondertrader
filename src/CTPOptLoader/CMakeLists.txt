cmake_minimum_required(VERSION 3.10)

project(CTPOptLoader LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 17)
# bin输出目录
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/Loader)

#7. 添加源码
SET(SRC  
	${PROJECT_SOURCE_DIR}/CTPOptLoader.cpp
	${PROJECT_SOURCE_DIR}/TraderSpi.cpp
	${PROJECT_SOURCE_DIR}/TraderSpi.h
)


add_library(CTPOptLoader SHARED ${SRC})
include_directories(${INCS})
link_directories(${LNKS})

SET(LIBS	
	# boost_filesystem
    ZTSUtils
)

# 链接，通用依赖
target_link_libraries(CTPOptLoader
    PRIVATE
        ${LIBS}
)

# 平台相关依赖
if(MSVC)
else
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


if (MSVC)
else (GNUCC)
	set_target_properties(CTPOptLoader PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		C_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
ENDIF ()