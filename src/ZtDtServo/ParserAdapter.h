/*!
 * \file ParserAdapter.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 行情解析模块适配类定义
 */
#pragma once
#include <set>
#include <vector>
#include <memory>
#include <boost/core/noncopyable.hpp>
#include "../Includes/IParserApi.h"

NS_ZTP_BEGIN
class ZTSVariant;
NS_ZTP_END

USING_NS_ZTP;
class ZTSBaseDataMgr;
class ZtDtRunner;

class ParserAdapter : public IParserSpi, private boost::noncopyable
{
public:
	ParserAdapter(ZTSBaseDataMgr * bgMgr, ZtDtRunner* runner);
	~ParserAdapter();

public:
	bool	init(const char* id, ZTSVariant* cfg);

	bool	initExt(const char* id, IParserApi* api);

	void	release();

	bool	run();

	const char* id() const { return _id.c_str(); }

public:
	virtual void handleSymbolList(const ZTSArray* aySymbols) override;

	virtual void handleQuote(ZTSTickData *quote, uint32_t procFlag) override;

	virtual void handleOrderQueue(ZTSOrdQueData* ordQueData) override;

	virtual void handleTransaction(ZTSTransData* transData) override;

	virtual void handleOrderDetail(ZTSOrdDtlData* ordDetailData) override;

	virtual void handleParserLog(ZTSLogLevel ll, const char* message) override;

	virtual IBaseDataMgr* getBaseDataMgr() override;

private:
	IParserApi*			_parser_api;
	FuncDeleteParser	_remover;
	ZTSBaseDataMgr*		_bd_mgr;
	ZtDtRunner*			_dt_runner;

	bool				_stopped;

	typedef vvt_hashset<std::string>	ExchgFilter;
	ExchgFilter			_exchg_filter;
	ExchgFilter			_code_filter;
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

	uint32_t size() const { return (uint32_t)_adapters.size(); }

public:
	ParserAdapterMap _adapters;
};


