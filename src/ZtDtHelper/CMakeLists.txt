cmake_minimum_required(VERSION 3.10)


project(ZtDtHelper LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/ZtDtPorter)

#源码
file(GLOB SRCS *.cpp)
file(GLOB HDRS *.h)

include_directories(${INCS})
link_directories(${LNKS})
add_library(ZtDtHelper SHARED ${SRCS} ${HDRS})

set(LIBS
	ZTSUtils
    ZTSTools
)
if(MSVC)
else()
	list(APPEND LIBS
		dl
		boost_filesystem
		)
endif()

target_link_libraries(ZtDtHelper PRIVATE ${LIBS})
if(MSVC)
else()
	set_target_properties(ZtDtHelper PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
        LINK_FLAGS_RELEASE -s)
endif()
		