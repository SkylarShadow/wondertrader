
cmake_minimum_required(VERSION 3.10)


project(TestExecPorter LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/ZtPorter)

#源码
file(GLOB SRCS *.cpp)

set(LIBS
	ZtExecMon
)

include_directories(${INCS})
link_directories(${LNKS})

add_executable(TestExecPorter ${SRCS})
target_link_libraries(TestExecPorter PRIVATE ${LIBS})

if(MSVC)
else()
	set_target_properties(TestExecPorter PROPERTIES
        LINK_FLAGS_RELEASE -s)
endif()