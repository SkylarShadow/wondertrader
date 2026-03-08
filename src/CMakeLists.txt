cmake_minimum_required(VERSION 3.16)

PROJECT(ZonTrader LANGUAGES CXX)

#C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 架构判断
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PLATFORM x64)
else()
    set(PLATFORM x86)
endif()


# ========= 编译器判断 =========
if(MSVC)
    message(STATUS "Compiler: MSVC")

    add_compile_definitions(
        _CRT_SECURE_NO_WARNINGS
        _HAS_STD_BYTE=0
    )

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Compiler: GNU")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Compiler: Clang")
endif()


#依赖
set(DEPS_DIR ${CMAKE_SOURCE_DIR}/../deps/v142)
set(INCS ${DEPS_DIR}/include)
set(LNKS ${DEPS_DIR}/lib/${PLATFORM})
set(BINS ${DEPS_DIR}/bin/${PLATFORM})

message(STATUS "Deps root: ${DEPS_DIR}")
message(STATUS "Deps Include: ${INCS}")
message(STATUS "Deps lib: ${LNKS}")


#库的前后缀
if (WIN32)	

	set(PREFIX )
	set(SUFFIX .dll)
	
elseif(UNIX)
    set(LNKS ${DEPS_DIR}/lib)
    set(PREFIX lib)
    set(SUFFIX .so)
else()
    set(PREFIX lib)
    set(SUFFIX .dll)
endif()

#include / link
include_directories(${INCS})
link_directories(${LNKS})

#basic libraries
add_subdirectory(ZTSUtils)
add_subdirectory(ZTSTools)

#contract Loade through ctp channel
add_subdirectory(CTPLoader)
add_subdirectory(CTPOptLoader)
add_subdirectory(LoaderRunner)

#parser for quote date from ctp channel
add_subdirectory(ParserCTP)
add_subdirectory(ParserCTPMini)
add_subdirectory(ParserCTPOpt)
add_subdirectory(ParserFemas)
add_subdirectory(ParserXTP)
add_subdirectory(ParserShm)
add_subdirectory(ParserXeleSkt)
add_subdirectory(ZtDataStorage)
add_subdirectory(ZtDataStorageAD)
add_subdirectory(ZtDtCore)
add_subdirectory(ZtDtHelper)
add_subdirectory(ZtDtPorter)
add_subdirectory(ZtDtServo)
#quote date processor
add_subdirectory(QuoteFactory)

#back test core library
add_subdirectory(ZtBtCore)
#back test c porter for outter libraries
add_subdirectory(ZtBtPorter)
add_subdirectory(ZtBtRunner)

add_subdirectory(ParserUDP)
add_subdirectory(TraderCTP)
add_subdirectory(TraderCTPMini)
add_subdirectory(TraderCTPOpt)
add_subdirectory(TraderFemas)
add_subdirectory(TraderMocker)
add_subdirectory(TraderXTP)
add_subdirectory(TraderYD)
add_subdirectory(ZtExeFact)
add_subdirectory(ZtRiskMonFact)

add_subdirectory(ZtMsgQue)
add_subdirectory(ZtCore)
add_subdirectory(ZtPorter)
add_subdirectory(ZtExecMon)
add_subdirectory(ZtRunner)

add_subdirectory(TraderDumper)

add_subdirectory(ZtShareHelper)

#uft
add_subdirectory(ZtUftCore)
add_subdirectory(ZtUftStraFact)
add_subdirectory(ZtUftRunner)

add_subdirectory(ZtLatencyHFT)
add_subdirectory(ZtLatencyUFT)

#test projects
add_subdirectory(TestBtPorter)
add_subdirectory(TestDtPorter)
add_subdirectory(TestExecPorter)
add_subdirectory(TestPorter)
add_subdirectory(TestTrader)
add_subdirectory(TestParser)
add_subdirectory(TestUnits)

