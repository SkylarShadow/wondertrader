
cmake_minimum_required(VERSION 3.10)


project(TestDtPorter LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin/ZtDtPorter)

#源码
file(GLOB SRCS *.cpp)

set(LIBS
	ZtDtPorter
	ZtDtHelper
)

include_directories(${INCS})
link_directories(${LNKS})

add_executable(TestDtPorter ${SRCS})
target_link_libraries(TestDtPorter PRIVATE ${LIBS})

if(MSVC)
else()
	set_target_properties(TestDtPorter PROPERTIES
        LINK_FLAGS_RELEASE -s)
endif()