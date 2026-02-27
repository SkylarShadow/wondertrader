CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

PROJECT(WonderTrader)

#3. 环境判断
IF (WIN32)
	MESSAGE(STATUS "Operation System is windows")
ELSEIF (UNIX)
	MESSAGE(STATUS "Operation System is UNIX-like OS's")
ENDIF ()

MESSAGE(STATUS "Generator is ${CMAKE_GENERATOR}")

IF (MSVC)
	ADD_DEFINITIONS (-D_CRT_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS (-D_HAS_STD_BYTE=0)
	SET(DEPS_DIR ${CMAKE_SOURCE_DIR}/../deps/v142)
	IF(CMAKE_CL_64)
		SET(INCS ${DEPS_DIR}/include)
		SET(LNKS ${DEPS_DIR}/lib/x64)
		SET(BINS ${DEPS_DIR}/bin/x64)

		MESSAGE(STATUS "Deps path: ${DEPS_DIR}")
        MESSAGE(STATUS "Include path: ${INCS}")
        MESSAGE(STATUS "Library path: ${LNKS}")
		MESSAGE(STATUS "Platform is x64")
		SET(PLATFORM x64)
	ELSE()
		SET(INCS ${DEPS_DIR}/include)
		SET(LNKS ${DEPS_DIR}/lib/x86)
		SET(BINS ${DEPS_DIR}/bin/x86)

        MESSAGE(STATUS "Deps path: ${DEPS_DIR}")
        MESSAGE(STATUS "Include path: ${INCS}")
        MESSAGE(STATUS "Library path: ${LNKS}")
        MESSAGE(STATUS "Platform: x86")
		SET(PLATFORM x86)
	ENDIF()
	SET(PREFIX )
	SET(SUFFIX .dll)
ELSE (GNUCC)

	SET(DEPS_DIR ${CMAKE_SOURCE_DIR}/../deps/v142)

	SET(INCS ${DEPS_DIR}/include)

	IF(WIN32)
		SET(LNKS ${DEPS_DIR}/lib/x64)#gcc是否要加x64未测试
		SET(BINS ${DEPS_DIR}/bin/x64)
		SET(SUFFIX .dll)
	ELSE(UNIX)
		SET(LNKS ${DEPS_DIR}/lib)
		SET(SUFFIX .so)
	ENDIF()
	
	MESSAGE(STATUS "Deps path: ${DEPS_DIR}")
    MESSAGE(STATUS "Include path: ${INCS}")
    MESSAGE(STATUS "Library path: ${LNKS}")
    MESSAGE(STATUS "Platform is x64")

	SET(PLATFORM x64)
	SET(PREFIX lib)
ENDIF()

SET(CMAKE_CXX_STANDARD 17)

#basic libraries
ADD_SUBDIRECTORY(WTSUtils)
ADD_SUBDIRECTORY(WTSTools)

#contract Loade through ctp channel
ADD_SUBDIRECTORY(CTPLoader)
ADD_SUBDIRECTORY(CTPOptLoader)
ADD_SUBDIRECTORY(LoaderRunner)

#parser for quote date from ctp channel
ADD_SUBDIRECTORY(ParserCTP)
ADD_SUBDIRECTORY(ParserCTPMini)
ADD_SUBDIRECTORY(ParserCTPOpt)
ADD_SUBDIRECTORY(ParserFemas)
ADD_SUBDIRECTORY(ParserXTP)
ADD_SUBDIRECTORY(ParserShm)
ADD_SUBDIRECTORY(ParserXeleSkt)
ADD_SUBDIRECTORY(WtDataStorage)
ADD_SUBDIRECTORY(WtDataStorageAD)
ADD_SUBDIRECTORY(WtDtCore)
ADD_SUBDIRECTORY(WtDtHelper)
ADD_SUBDIRECTORY(WtDtPorter)
ADD_SUBDIRECTORY(WtDtServo)
#quote date processor
ADD_SUBDIRECTORY(QuoteFactory)

#back test core library
ADD_SUBDIRECTORY(WtBtCore)
#back test c porter for outter libraries
ADD_SUBDIRECTORY(WtBtPorter)
ADD_SUBDIRECTORY(WtBtRunner)

ADD_SUBDIRECTORY(ParserUDP)
ADD_SUBDIRECTORY(TraderCTP)
ADD_SUBDIRECTORY(TraderCTPMini)
ADD_SUBDIRECTORY(TraderCTPOpt)
ADD_SUBDIRECTORY(TraderFemas)
ADD_SUBDIRECTORY(TraderMocker)
ADD_SUBDIRECTORY(TraderXTP)
ADD_SUBDIRECTORY(TraderYD)
ADD_SUBDIRECTORY(WtExeFact)
ADD_SUBDIRECTORY(WtRiskMonFact)

ADD_SUBDIRECTORY(WtMsgQue)
ADD_SUBDIRECTORY(WtCore)
ADD_SUBDIRECTORY(WtPorter)
ADD_SUBDIRECTORY(WtExecMon)
ADD_SUBDIRECTORY(WtRunner)

ADD_SUBDIRECTORY(TraderDumper)

ADD_SUBDIRECTORY(WtShareHelper)

#uft
ADD_SUBDIRECTORY(WtUftCore)
ADD_SUBDIRECTORY(WtUftStraFact)
ADD_SUBDIRECTORY(WtUftRunner)

ADD_SUBDIRECTORY(WtLatencyHFT)
ADD_SUBDIRECTORY(WtLatencyUFT)

#test projects
ADD_SUBDIRECTORY(TestBtPorter)
ADD_SUBDIRECTORY(TestDtPorter)
ADD_SUBDIRECTORY(TestExecPorter)
ADD_SUBDIRECTORY(TestPorter)
ADD_SUBDIRECTORY(TestTrader)
ADD_SUBDIRECTORY(TestParser)
ADD_SUBDIRECTORY(TestUnits)

