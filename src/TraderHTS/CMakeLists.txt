
cmake_minimum_required(VERSION 3.10)

#2. 确定工程名
project(TraderHTS LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)


if (WIN32)
	set(INCS ${DEPS_DIR}/include)
	set(LNKS ${DEPS_DIR}/lib/${PLATFORM})
else (UNIX)
	set(INCS ${DEPS_DIR}/include)
	set(LNKS ${DEPS_DIR}/lib) #待测试
endif ()

set(SRC  
	${PROJECT_SOURCE_DIR}/TraderHTS.cpp
	${PROJECT_SOURCE_DIR}/TraderHTS.h
)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build_${PLATFORM}/${CMAKE_BUILD_TYPE}/bin)

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 
set(CMAKE_INSTALL_RPATH "\${ORIGIN}")

include_directories(${INCS})
add_library(TraderHTS SHARED ${SRC})

if (WIN32)
else (UNIX)
	target_link_libraries(TraderHTS PRIVATE ${PROJECT_SOURCE_DIR}/FixApi/libfixapi.so )
	set_target_properties(TraderHTS PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		C_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1)
endif ()

