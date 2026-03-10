
cmake_minimum_required(VERSION 3.10)


project(TestBtPorter LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/ZtBtPorter)

#源码
file(GLOB SRCS *.cpp)

set(LIBS
	ZtBtPorter
)

include_directories(${INCS})
link_directories(${LNKS})

add_executable(TestBtPorter ${SRCS})
target_link_libraries(TestBtPorter PRIVATE ${LIBS})

if(MSVC)
else()
	set_target_properties(TestBtPorter PROPERTIES
        LINK_FLAGS_RELEASE -s)
endif()