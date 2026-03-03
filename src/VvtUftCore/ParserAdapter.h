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


NS_VVTP_BEGIN
class VvTSVariant;

class IParserStub
{
public:
	virtual void			handle_push_quote(VvTSTickData* curTick){}

	virtual void			handle_push_order_detail(VvTSOrdDtlData* curOrdDtl){}
	virtual void			handle_push_order_queue(VvTSOrdQueData* curOrdQue) {}
	virtual void			handle_push_transaction(VvTSTransData* curTrans) {}
};

class ParserAdapter : public IParserSpi,
					private boost::noncopyable
{
public:
	ParserAdapter();
	~ParserAdapter();

public:
	bool	init(const char* id, VvTSVariant* cfg, IParserStub* stub, IBaseDataMgr* bgMgr);
	bool	initExt(const char* id, IParserApi* api, IParserStub* stub, IBaseDataMgr* bgMgr);

	void	release();

	bool	run();

	const char* id() const{ return _id.c_str(); }

public:
	virtual void handleSymbolList(const VvTSArray* aySymbols) override {}

	/*
	 *	处理实时行情
	 *	@quote		实时行情
	 *	@bNeedSlice	是否需要切片,如果是从外部接入的快照行情数据,则需要切片,如果是内部广播的就不需要切片
	 */
	virtual void handleQuote(VvTSTickData *quote, uint32_t procFlag) override;

	/*
	 *	处理委托队列数据（股票level2）
	 *	@ordQueData	委托对垒数据
	 */
	virtual void handleOrderQueue(VvTSOrdQueData* ordQueData) override;

	/*
	 *	处理逐笔委托数据（股票level2）
	 *	@ordDetailData	逐笔委托数据
	 */
	virtual void handleOrderDetail(VvTSOrdDtlData* ordDetailData) override;

	/*
		*	处理逐笔成交数据
		*	@transData	逐笔成交数据
		*/
	virtual void handleTransaction(VvTSTransData* transData) override;

	virtual void handleParserLog(VvTSLogLevel ll, const char* message) override;

	virtual IBaseDataMgr* getBaseDataMgr() override { return _bd_mgr; }


private:
	IParserApi*			_parser_api;
	FuncDeleteParser	_remover;

	bool				_stopped;

	typedef vvt_hashset<std::string>	ExchgFilter;
	ExchgFilter			_exchg_filter;
	ExchgFilter			_code_filter;
	IBaseDataMgr*		_bd_mgr;
	IParserStub*		_stub;
	VvTSVariant*			_cfg;
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

NS_VVTP_END