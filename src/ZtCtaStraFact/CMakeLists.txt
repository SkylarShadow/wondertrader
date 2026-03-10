
cmake_minimum_required(VERSION 3.10)

#2. 确定工程名
project(ZtCtaStraFact LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

#7. 添加源码
file(GLOB SRCS "*.cpp")
file(GLOB HDRS "*.h")

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin)

include_directories(${INCS})
link_directories(${LNKS})
add_library(ZtCtaStraFact SHARED ${SRCS} ${HDRS})

set(LIBS
	boost_filesystem
	dl
)

if (WIN32)
else (UNIX)
	target_link_libraries(ZtCtaStraFact PRIVATE ${LIBS})
	set_target_properties(ZtCtaStraFact PROPERTIES
		CXX_VISIBILITY_PREsethidden
		C_VISIBILITY_PREsethidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif ()

