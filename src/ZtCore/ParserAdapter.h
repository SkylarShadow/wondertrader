/*!
 * \file ParserAdapter.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <memory>
#include <boost/core/noncopyable.hpp>

#include "../Includes/FasterDefs.h"
#include "../Includes/IParserApi.h"


NS_ZTP_BEGIN
class ZTSVariant;
class IHotMgr;

class IParserStub
{
public:
	virtual void			handle_push_quote(ZTSTickData* curTick){}

	virtual void			handle_push_order_detail(ZTSOrdDtlData* curOrdDtl){}
	virtual void			handle_push_order_queue(ZTSOrdQueData* curOrdQue) {}
	virtual void			handle_push_transaction(ZTSTransData* curTrans) {}
};

class ParserAdapter : public IParserSpi,
					private boost::noncopyable
{
public:
	ParserAdapter();
	~ParserAdapter();

public:
	bool	init(const char* id, ZTSVariant* cfg, IParserStub* stub, IBaseDataMgr* bgMgr, IHotMgr* hotMgr = NULL);

	bool	initExt(const char* id, IParserApi* api, IParserStub* stub, IBaseDataMgr* bgMgr, IHotMgr* hotMgr = NULL);

	void	release();

	bool	run();

	const char* id() const{ return _id.c_str(); }

public:
	virtual void handleSymbolList(const ZTSArray* aySymbols) override {}

	/*
	 *	处理实时行情
	 *	@quote		实时行情
	 *	@bNeedSlice	是否需要切片,如果是从外部接入的快照行情数据,则需要切片,如果是内部广播的就不需要切片
	 */
	virtual void handleQuote(ZTSTickData *quote, uint32_t procFlag) override;

	/*
	 *	处理委托队列数据（股票level2）
	 *	@ordQueData	委托对垒数据
	 */
	virtual void handleOrderQueue(ZTSOrdQueData* ordQueData) override;

	/*
	 *	处理逐笔委托数据（股票level2）
	 *	@ordDetailData	逐笔委托数据
	 */
	virtual void handleOrderDetail(ZTSOrdDtlData* ordDetailData) override;

	/*
		*	处理逐笔成交数据
		*	@transData	逐笔成交数据
		*/
	virtual void handleTransaction(ZTSTransData* transData) override;

	virtual void handleParserLog(ZTSLogLevel ll, const char* message) override;

	virtual IBaseDataMgr* getBaseDataMgr() override { return _bd_mgr; }


private:
	IParserApi*			_parser_api;
	FuncDeleteParser	_remover;

	bool				_stopped;

	/*
	 *	检查时间设置项
	 *	如果为true，则在收到行情的时候进行时间检查
	 *	主要适用于直接从行情源接入
	 *	因为直接从行情源接入很可能会有错误时间戳的数据进来
	 *	该选项默认为false
	 */
	bool				_check_time;

	typedef vvt_hashset<std::string>	ExchgFilter;
	ExchgFilter			_exchg_filter;
	ExchgFilter			_code_filter;
	IBaseDataMgr*		_bd_mgr;
	IHotMgr*			_hot_mgr;
	IParserStub*		_stub;
	ZTSVariant*			_cfg;
	std::string			_id;
};

typedef std::shared_ptr<ParserAdapter>	ParserAdapterPtr;
typedef vvt_hashmap<std::string, ParserAdapterPtr>	ParserAdapterMap;

class ParserAdapterMgr : private boost::noncopyable
{
public:
	void	release();

	void	run();

	ParserAdapterPtr getAdapter(const char* id);

	bool	addAdapter(const char* id, ParserAdapterPtr& adapter);


public:
	ParserAdapterMap _adapters;
};

NS_ZTP_END