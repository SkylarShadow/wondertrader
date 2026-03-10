cmake_minimum_required(VERSION 3.10)

project(LoaderRunner LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/Loader)

set(LIBS
	ZtBtCore
	ZTSTools
	ZTSUtils
)

#添加源码
file(GLOB SRCS *.cpp)
file(GLOB HDRS *.h)


include_directories(${INCS})
link_directories(${LNKS})

add_executable(LoaderRunner ${SRCS} ${HDRS})

target_link_libraries(LoaderRunner 
    PRIVATE
        ${LIBS}
)

if(MSVC)
else()
	if(WIN32)
	    target_link_libraries(LoaderRunner
            PRIVATE
                dl
        )
    endif()
    target_link_libraries(LoaderRunner
        PRIVATE
            iconv
    )
endif()


if(MSVC)
else()
	set_target_properties(LoaderRunner PROPERTIES
        LINK_FLAGS_RELEASE -s)
endif()

#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/ZtBtPorter)
#add_executable(TestBtPorter ../TestBtPorter/TestBtPorter.cpp)
#target_link_libraries(TestBtPorter ${LIBS})

