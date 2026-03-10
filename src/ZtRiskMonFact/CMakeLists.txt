cmake_minimum_required(VERSION 3.10)


project(ZtRiskMonFact LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

#源码
file(GLOB SRCS *.cpp)
file(GLOB HDRS *.h)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin)

include_directories(${INCS})
link_directories(${LNKS})
add_library(ZtRiskMonFact SHARED ${SRCS} ${HDRS})

if(MSVC)
else()
	
	set_target_properties(ZtRiskMonFact PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		C_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif()

