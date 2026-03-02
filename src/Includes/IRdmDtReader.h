/*!
 * \file IRdmDtReader.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <stdint.h>

#include "../Includes/VvTSMarcos.h"
#include "../Includes/VvTSTypes.h"

NS_VVTP_BEGIN
class VvTSKlineSlice;
class VvTSTickSlice;
class VvTSOrdQueSlice;
class VvTSOrdDtlSlice;
class VvTSTransSlice;
class IBaseDataMgr;
class IHotMgr;
class VvTSVariant;


/*
 *	@brief 数据读取模块回调接口
 *	@details 主要用于数据读取模块向调用模块回调
 */
class IRdmDtReaderSink
{
public:
	/*
	 *	@brief	获取基础数据管理接口指针
	 */
	virtual IBaseDataMgr*	get_basedata_mgr() = 0;

	/*
	 *	@brief	获取主力切换规则管理接口指针
	 */
	virtual IHotMgr*		get_hot_mgr() = 0;

	/*
	 *	@brief	输出数据读取模块的日志
	 */
	virtual void		reader_log(VvTSLogLevel ll, const char* message) = 0;
};

/*
 *	@brief	随机数据读取接口
 *
 *	向核心模块提供行情数据(tick、K线)读取接口
 */
class IRdmDtReader
{
public:
	IRdmDtReader() :_sink(NULL) {}
	virtual ~IRdmDtReader(){}

public:
	virtual void init(VvTSVariant* cfg, IRdmDtReaderSink* sink) { _sink = sink; }

	virtual VvTSOrdDtlSlice*	readOrdDtlSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) = 0;
	virtual VvTSOrdQueSlice*	readOrdQueSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) = 0;
	virtual VvTSTransSlice*	readTransSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) = 0;

	virtual VvTSTickSlice*	readTickSliceByDate(const char* stdCode, uint32_t uDate = 0) = 0;

	virtual VvTSTickSlice*	readTickSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) = 0;
	virtual VvTSKlineSlice*	readKlineSliceByRange(const char* stdCode, VvTSKlinePeriod period, uint64_t stime, uint64_t etime = 0) = 0;

	virtual VvTSTickSlice*	readTickSliceByCount(const char* stdCode, uint32_t count, uint64_t etime = 0) = 0;
	virtual VvTSKlineSlice*	readKlineSliceByCount(const char* stdCode, VvTSKlinePeriod period, uint32_t count, uint64_t etime = 0) = 0;

	/*
	 *	@brief 获取个股指定日期的复权因子
	 *
	 *	@param	stdCode	标准品种代码,如SSE.600000
	 *	@param	date	指定日期,格式yyyyMMdd，默认为0，为0则按当前日期处理
	 */
	virtual double		getAdjFactorByDate(const char* stdCode, uint32_t date = 0) { return 1.0; }

	virtual void		clearCache(){}

protected:
	IRdmDtReaderSink*	_sink;
};

//创建数据存储对象
typedef IRdmDtReader* (*FuncCreateRdmDtReader)();
//删除数据存储对象
typedef void(*FuncDeleteRdmDtReader)(IRdmDtReader* store);

NS_VVTP_END