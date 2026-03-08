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

#include "../Includes/ZTSMarcos.h"
#include "../Includes/ZTSTypes.h"

NS_ZTP_BEGIN
class ZTSKlineSlice;
class ZTSTickSlice;
class ZTSOrdQueSlice;
class ZTSOrdDtlSlice;
class ZTSTransSlice;
class IBaseDataMgr;
class IHotMgr;
class ZTSVariant;


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
	virtual void		reader_log(ZTSLogLevel ll, const char* message) = 0;
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
	virtual void init(ZTSVariant* cfg, IRdmDtReaderSink* sink) { _sink = sink; }

	virtual ZTSOrdDtlSlice*	readOrdDtlSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) = 0;
	virtual ZTSOrdQueSlice*	readOrdQueSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) = 0;
	virtual ZTSTransSlice*	readTransSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) = 0;

	virtual ZTSTickSlice*	readTickSliceByDate(const char* stdCode, uint32_t uDate = 0) = 0;

	virtual ZTSTickSlice*	readTickSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) = 0;
	virtual ZTSKlineSlice*	readKlineSliceByRange(const char* stdCode, ZTSKlinePeriod period, uint64_t stime, uint64_t etime = 0) = 0;

	virtual ZTSTickSlice*	readTickSliceByCount(const char* stdCode, uint32_t count, uint64_t etime = 0) = 0;
	virtual ZTSKlineSlice*	readKlineSliceByCount(const char* stdCode, ZTSKlinePeriod period, uint32_t count, uint64_t etime = 0) = 0;

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

NS_ZTP_END