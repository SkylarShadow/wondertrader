cmake_minimum_required(VERSION 3.10)


project(ZtUftStraFact LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

#源码
file(GLOB SRCS "*.cpp")
file(GLOB HDRS "*.h")

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/ZtUftRunner)

include_directories(${INCS})
link_directories(${LNKS})
add_library(ZtUftStraFact SHARED ${SRCS} ${HDRS})

set(LIBS
	boost_filesystem
	dl
)

if (WIN32)
ELSE (UNIX)
	target_link_libraries(ZtUftStraFact PRIVATE ${LIBS})
	set_target_properties(ZtUftStraFact PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		C_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif()

