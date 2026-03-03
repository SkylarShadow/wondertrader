#include <iostream>
#include <boost/filesystem.hpp>

#include "../Includes/ITraderApi.h"
#include "../Includes/VvTSVariant.hpp"
#include "../Includes/VvTSTradeDef.hpp"
#include "../Includes/VvTSError.hpp"
#include "../Includes/VvTSCollection.hpp"

#include "../Share/TimeUtils.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/charconv.hpp"

#include "../VvTSTools/VvTSBaseDataMgr.h"
#include "../VvTSTools/VvTSLogger.h"
#include "../VvTSUtils/VvTSCfgLoader.h"


VvTSBaseDataMgr	g_bdMgr;
StdUniqueMutex	g_mtxOpt;
StdCondVariable	g_condOpt;
bool		g_exitNow = false;
bool		g_riskAct = false;
std::set<std::string>	g_blkList;

template<typename... Args>
inline void encoding_print(const char* format, const Args& ...args)
{
	fmt::print(format, args...);
	//printf(fmtutil::format(format, args...));
//#ifdef _MSC_VER
//	printf(UTF8toChar(s).c_str());
//#else
//	printf(s.c_str());
//#endif
}

USING_NS_VVTP;

class TraderSpi : public ITraderSpi
{
public:
	TraderSpi() :m_bLogined(false), m_mapOrds(NULL){}

	bool init(VvTSVariant* params, const char* ttype)
	{
		m_pParams = params;
		if (m_pParams)
			m_pParams->retain();

		m_strModule = ttype;
		return true;
	}

	void release()
	{
		if (m_pTraderApi)
		{
			m_pTraderApi->release();
		}
	}

	void run(bool bRestart = false)
	{
		if (!createTrader(m_strModule.c_str()))
		{
			return;
		}

		m_pTraderApi->registerSpi(this);

		if (!m_pTraderApi->init(m_pParams))
		{
			return;
		}

		m_pTraderApi->connect();
	}

	bool createTrader(const char* moduleName)
	{
		DllHandle hInst = DLLHelper::load_library(moduleName);
		if (hInst == NULL)
		{
			VvTSLogger::info("Loading module {} failed", moduleName);
			return false;
		}

		FuncCreateTrader pFunCreateTrader = (FuncCreateTrader)DLLHelper::get_symbol(hInst, "createTrader");
		if (NULL == pFunCreateTrader)
		{
			VvTSLogger::info("Entry function createTrader not exists");
			return false;
		}

		m_pTraderApi = pFunCreateTrader();
		if (NULL == m_pTraderApi)
		{
			VvTSLogger::info("Creating trader api failed");
			return false;
		}

		m_funcDelTrader = (FuncDeleteTrader)DLLHelper::get_symbol(hInst, "deleteTrader");
		return true;
	}

	bool qryFund()
	{
		VvTSLogger::info("Querying fund info...");
		m_pTraderApi->queryAccount();

		return true;
	}

	bool qryOrders()
	{
		VvTSLogger::info("Querying orders...");
		m_pTraderApi->queryOrders();

		return true;
	}

	bool qryTrades()
	{
		VvTSLogger::info("Querying trades...");
		m_pTraderApi->queryTrades();

		return true;
	}

	bool qryPosition()
	{
		VvTSLogger::info("Querying positions...");
		m_pTraderApi->queryPositions();

		return true;
	}

	bool qrySettle()
	{
		uint32_t uDate = TimeUtils::getNextDate(TimeUtils::getCurDate(), -1);
		VvTSLogger::info("Querying settlement info on {}...", uDate);
		m_pTraderApi->querySettlement(uDate);

		return true;
	}

	bool entrustLmt(bool isNet)
	{
		char code[32] = { 0 };
		char exchg[32] = { 0 };
		double price = 0.0;
		double qty = 0;
		uint32_t bs = 0;
		uint32_t offset = 0;

		for (;;)
		{
			encoding_print("合约代码: ");
			std::cin >> code;

			encoding_print("交易所代码: ");
			std::cin >> exchg;

			encoding_print("委托价格: ");
			std::cin >> price;

			encoding_print("委托数量: ");
			std::cin >> qty;

			if(isNet)
			{
				encoding_print("方向: 0-买, 1-卖: ");
				std::cin >> bs;
				if (bs != 0 && bs != 1)
					continue;

				encoding_print("合约: {}.{},价格: {},数量: {},方向: {},确认(y/n)? ", exchg, code, price, qty, bs == 0 ? "买" : "卖");
			}
			else
			{
				encoding_print("方向: 0-多, 1-空: ");
				std::cin >> bs;
				if (bs != 0 && bs != 1)
					continue;

				encoding_print("开平: 0-开, 1-平,2-平今: ");
				std::cin >> offset;
				if (offset != 0 && offset != 1 && offset != 2)
					continue;

				encoding_print("合约: {}.{},价格: {},数量: {},方向: {},开盘: {},确认(y/n)? ", exchg, code, price, qty, bs == 0 ? "多" : "空", offset == 0 ? "开" : "平");
			}
			
			char c;
			std::cin >> c;
			if(c == 'y')
				break;
		}

		if(g_riskAct)
		{
			auto it = g_blkList.find(code);
			if (it != g_blkList.end())
			{
				VvTSLogger::info("{}已被禁止交易", code);
				return false;
			}
		}

		bool bNeedToday = (strcmp(exchg, "SHFE") == 0 || strcmp(exchg, "INE") == 0);

		VvTSEntrust* entrust = VvTSEntrust::create(code, qty, price, exchg);
		if(!isNet)
		{
			entrust->setDirection(bs == 0 ? WDT_LONG : WDT_SHORT);
			entrust->setOffsetType(offset == 0 ? WOT_OPEN : (bNeedToday ? WOT_CLOSETODAY : WOT_CLOSE));
		}
		else
		{
			entrust->setNetDirection(bs == 0);
		}
		
		entrust->setPriceType(WPT_LIMITPRICE);
		entrust->setOrderFlag(WOF_NOR);

		char entrustid[64] = { 0 };
		m_pTraderApi->makeEntrustID(entrustid, 64);
		entrust->setEntrustID(entrustid);
		entrust->setUserTag("test");

		if(!isNet)
			VvTSLogger::info("[{}]Entrusting,Contract: {}.{},Price: {},Volume: {},Action: {}{}", m_pParams->getCString("user"), exchg, code, price, qty, offset == 0 ? "Open" : "Close", bs == 0 ? "Long" : "Short");
		else
			VvTSLogger::info("[{}]Entrusting,Contract: {}.{},Price: {},Volume: {},Action: {}", m_pParams->getCString("user"), exchg, code, price, qty, bs == 0 ? "buy" : "sell");

		entrust->setContractInfo(g_bdMgr.getContract(code, exchg));
		m_pTraderApi->orderInsert(entrust);
		entrust->release();

		return true;
	}

	bool entrustMkt()
	{
		char code[32] = { 0 };
		char exchg[32] = { 0 };
		uint32_t qty = 0;
		uint32_t bs = 0;
		uint32_t offset = 0;

		for (;;)
		{
			encoding_print("合约代码: ");
			std::cin >> code;

			encoding_print("交易所代码: ");
			std::cin >> exchg;

			encoding_print("委托数量: ");
			std::cin >> qty;

			encoding_print("方向: 0-多, 1-空: ");
			std::cin >> bs;

			encoding_print("开平: 0-开, 1-平: ");
			std::cin >> offset;

			encoding_print("合约: {}.{},数量: {},方向: {},开盘: {},确认(y/n)? ", exchg, code, qty, bs == 0 ? "多" : "空", offset == 0 ? "开" : "平");
			char c;
			std::cin >> c;
			if (c == 'y')
				break;
		}

		if (g_riskAct)
		{
			auto it = g_blkList.find(code);
			if (it != g_blkList.end())
			{
				VvTSLogger::info("{}已被禁止交易", code);
				return false;
			}
		}

		bool bNeedToday = (strcmp(exchg, "SHFE") == 0 || strcmp(exchg, "INE") == 0);
		VvTSEntrust* entrust = VvTSEntrust::create(code, qty, 0, exchg);
		entrust->setDirection(bs == 0 ? WDT_LONG : WDT_SHORT);
		entrust->setOffsetType(offset == 0 ? WOT_OPEN : (bNeedToday ? WOT_CLOSETODAY : WOT_CLOSE));
		entrust->setPriceType(WPT_ANYPRICE);
		entrust->setOrderFlag(WOF_NOR);

		char entrustid[64] = { 0 };
		m_pTraderApi->makeEntrustID(entrustid, 64);
		entrust->setEntrustID(entrustid);

		VvTSLogger::info("[{}]Entrusting,Contract: {}.{},Price: MarketPx,Volume: {},Action: {}{}", m_pParams->getCString("user"), exchg, code, qty, offset == 0 ? "Open" : "Close", bs == 0 ? "Long" : "Short");

		m_pTraderApi->orderInsert(entrust);
		entrust->release();

		return true;
	}

	bool cancel()
	{
		std::string orderid;

		for (;;)
		{
			encoding_print("订单号: ");
			std::cin >> orderid;

			StrUtil::replace(orderid, "\"", "");

			encoding_print("订单号: {},确认撤单(y/n)? ", orderid);
			char c;
			std::cin >> c;
			if (c == 'y')
				break;
		}

		if (m_mapOrds == NULL)
			m_mapOrds = VvTSObjectMap::create();

		VvTSOrderInfo* ordInfo = (VvTSOrderInfo*)m_mapOrds->get(orderid);
		if (ordInfo == NULL)
		{
			VvTSLogger::info("Order not exists, Check your orders or query orders first");
			return false;
		}


		VvTSLogger::info("[{}]Canceling [{}]...", m_pParams->getCString("user"), orderid);
		VvTSEntrustAction* action = VvTSEntrustAction::create(ordInfo->getCode(), ordInfo->getExchg());
		action->setEntrustID(ordInfo->getEntrustID());
		action->setOrderID(ordInfo->getOrderID());
		action->setActionFlag(WAF_CANCEL);

		m_pTraderApi->orderAction(action);
		action->release();

		return true;
	}

public:
	virtual void handleEvent(VvTSTraderEvent e, int32_t ec)
	{
		if(e == VvTE_Connect)
		{
			if (ec == 0)
			{
				VvTSLogger::info("[{}] Connected", m_pParams->getCString("user"));
				m_pTraderApi->login(m_pParams->getCString("user"), m_pParams->getCString("pass"), "");
			}
			else
			{
				g_exitNow = true;
				StdUniqueLock lock(g_mtxOpt);
				g_condOpt.notify_all();
			}
		}
	}

	virtual void handleTraderLog(VvTSLogLevel ll, const char* message) override
	{
		VvTSLogger::log_raw(ll, message);
	}

	virtual void onLoginResult(bool bSucc, const char* msg, uint32_t tradingdate)
	{
		if(bSucc)
		{
			VvTSLogger::info("[{}] Login Succ" , m_pParams->getCString("user"));
			m_bLogined = true;
		}
		else
		{
			VvTSLogger::info("[{}] Login Fail: {}", m_pParams->getCString("user"), msg);
			g_exitNow = true;
		}

		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspEntrust(VvTSEntrust* entrust, VvTSError *err)
	{
		if(err)
		{
			VvTSLogger::info("[{}] Entrust fail: {}", m_pParams->getCString("user"), err->getMessage());
			StdUniqueLock lock(g_mtxOpt);
			g_condOpt.notify_all();
		}
		
	}

	virtual void onRspAccount(VvTSArray* ayAccounts)
	{
		if(ayAccounts != NULL)
		{
			VvTSAccountInfo* accInfo = (VvTSAccountInfo*)ayAccounts->at(0);
			if(accInfo)
			{
				VvTSLogger::info("[{}]Fund data updated, balance: {:.2f}", m_pParams->getCString("user"), accInfo->getBalance());
			}
		}

		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspPosition(const VvTSArray* ayPositions)
	{
		uint32_t cnt = 0;
		if (ayPositions != NULL)
			cnt = ayPositions->size();

		VvTSLogger::info("[{}] Positions updated, {} item totally", m_pParams->getCString("user"), cnt);
		for(uint32_t i = 0; i < cnt; i++)
		{
			VvTSPositionItem* posItem = (VvTSPositionItem*)((VvTSArray*)ayPositions)->at(i);
			if(posItem && posItem->getTotalPosition() > 0)
			{
				if(g_riskAct)
				{
					g_blkList.insert(posItem->getCode());
					VvTSLogger::info("{}持仓量超限,限制open", posItem->getCode());
				}
				
				VvTSLogger::info("Position of {}({}) updated, {}[{}]", posItem->getCode(), posItem->getDirection() == WDT_LONG ? "L" : "S", posItem->getTotalPosition(), posItem->getAvailPosition());
			}
		}
		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspOrders(const VvTSArray* ayOrders)
	{
		uint32_t cnt = 0;
		if (ayOrders != NULL)
			cnt = ayOrders->size();

		if (m_mapOrds == NULL)
			m_mapOrds = VvTSObjectMap::create();

		m_mapOrds->clear();
		for (uint32_t i = 0; i < cnt; i++)
		{
			VvTSOrderInfo* ordInfo = (VvTSOrderInfo*)((VvTSArray*)ayOrders)->at(i);
			if (ordInfo->isAlive())
			{
				m_mapOrds->add(StrUtil::trim(ordInfo->getOrderID()), ordInfo, true);
				VvTSLogger::info("[{}] Live order, code: {}, OrderId: {}", m_pParams->getCString("user"), ordInfo->getCode(), ordInfo->getOrderID());
			}
		}

		VvTSLogger::info("[{}] Orders updated, {} orders totally, {} orders live", m_pParams->getCString("user"), cnt, m_mapOrds->size());

		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspTrades(const VvTSArray* ayTrades)
	{
		uint32_t cnt = 0;
		if (ayTrades != NULL)
			cnt = ayTrades->size();

		VvTSLogger::info("[{}] Trades updates, {} trades totally", m_pParams->getCString("user"), cnt);
		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspSettlementInfo(uint32_t uDate, const char* content)
	{
		VvTSLogger::info("[{}]{} Settlement received", m_pParams->getCString("user"), uDate);
		VvTSLogger::info(content);
		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onPushOrder(VvTSOrderInfo* orderInfo)
	{
		std::string orderid = StrUtil::trim(orderInfo->getOrderID());
		if(orderInfo->getOrderState() != WOS_Canceled)
		{
			if(!orderid.empty())
			{
				if (m_mapOrds == NULL)
					m_mapOrds = VvTSObjectMap::create();

				if (m_mapOrds->find(orderid) == m_mapOrds->end())
				{
					VvTSLogger::info("[{}] Entrust Success,OrderID: {}",  m_pParams->getCString("user"), orderid);
					m_mapOrds->add(orderid, orderInfo, true);
				}

				StdUniqueLock lock(g_mtxOpt);
				g_condOpt.notify_all();
			}
		}
		else 
		{
			if (m_mapOrds)
				m_mapOrds->remove(orderid);

			if (orderid.empty())
			{
				VvTSLogger::info("[{}]Order {} Entrust failed and canceld:{}", m_pParams->getCString("user"), orderInfo->getEntrustID(), orderInfo->getStateMsg());
				StdUniqueLock lock(g_mtxOpt);
				g_condOpt.notify_all();
			}
			else
			{
				VvTSLogger::info("[{}] Order {} canceld:{}", m_pParams->getCString("user"), orderid, orderInfo->getStateMsg());
				StdUniqueLock lock(g_mtxOpt);
				g_condOpt.notify_all();
			}			
		}
	}

	virtual void onPushTrade(VvTSTradeInfo* tradeRecord)
	{
		VvTSLogger::info("[{}] Trade pushed,contract: {},price: {},Volume: {}", m_pParams->getCString("user"), tradeRecord->getCode(), tradeRecord->getPrice(), tradeRecord->getVolume());

		if(g_riskAct)
		{
			VvTSLogger::info("[{}]{}超过最大持仓Volume,禁止open", m_pParams->getCString("user"), tradeRecord->getCode());

			g_blkList.insert(tradeRecord->getCode());
		}
	}

	virtual void onTraderError(VvTSError*	err)
	{
		if(err && err->getErrorCode() == WEC_ORDERCANCEL)
		{
			VvTSLogger::info("[{}] Canceling failed: {}", m_pParams->getCString("user"), err->getMessage());
			StdUniqueLock lock(g_mtxOpt);
			g_condOpt.notify_all();
		}
		else if (err && err->getErrorCode() == WEC_ORDERINSERT)
		{
			VvTSLogger::info("[{}] Entrust failed: {}", m_pParams->getCString("user"), err->getMessage());
			StdUniqueLock lock(g_mtxOpt);
			g_condOpt.notify_all();
		}
	}

public:
	virtual IBaseDataMgr*	getBaseDataMgr()
	{
		return &g_bdMgr;
	}
	

private:
	ITraderApi*			m_pTraderApi;
	FuncDeleteTrader	m_funcDelTrader;
	std::string			m_strModule;
	VvTSVariant*			m_pParams;

	typedef VvTSHashMap<std::string>	VvTSObjectMap;
	VvTSObjectMap*		m_mapOrds;

	bool				m_bLogined;
};

std::string getBaseFolder()
{
	static std::string basePath;
	if (basePath.empty())
	{
		basePath = boost::filesystem::initial_path<boost::filesystem::path>().string();

		basePath = StrUtil::standardisePath(basePath);
	}

	return basePath.c_str();
}

int main()
{
	VvTSLogger::init("logcfg.yaml");

	VvTSVariant* root = VvTSCfgLoader::load_from_file("config.yaml");
	if(root == NULL)
	{
		VvTSLogger::log_raw(LL_ERROR, "配置文件config.yaml加载失败");
		return 0;
	}

	VvTSVariant* cfg = root->get("config");
	bool isUTF8 = cfg->getBoolean("utf8");
	if(cfg->has("session"))
		g_bdMgr.loadSessions(cfg->getCString("session"));

	if (cfg->has("commodity"))
		g_bdMgr.loadCommodities(cfg->getCString("commodity"));

	if (cfg->has("contract"))
		g_bdMgr.loadContracts(cfg->getCString("contract"));

	if (cfg->has("holiday"))
		g_bdMgr.loadHolidays(cfg->getCString("holiday"));

	g_riskAct = cfg->getBoolean("risk");
	VvTSLogger::info("RiskMon: {}", g_riskAct ? "Open" : "Closed");

	std::string module = cfg->getCString("trader");
	std::string profile = cfg->getCString("profile");
	VvTSVariant* params = root->get(profile.c_str());
	if(params == NULL)
	{
		VvTSLogger::error("配置项{}不存在", profile);
		return 0;
	}

	TraderSpi* trader = new TraderSpi;
	trader->init(params, module.c_str());
	trader->run();

	root->release();

	{
		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.wait(lock);
	}
	
	while(true)
	{
		encoding_print("Please pick an action\r\n");
		encoding_print("1. 查询资金\r\n");
		encoding_print("2. 查询订单\r\n");
		encoding_print("3. 查询成交\r\n");
		encoding_print("4. 查询持仓\r\n");
		encoding_print("5. 查询结算单\r\n");
		encoding_print("6. 限价委托\r\n");
		encoding_print("7. 市价委托\r\n");
		encoding_print("8. 撤单\r\n");
		encoding_print("9. 净头寸交易\r\n");
		encoding_print("0. 退出\r\n");

		char cmd;
		for (;;)
		{
			scanf("%c", &cmd);

			if(cmd >= '0' && cmd <= '9')
				break;
		}

		bool bSucc = false;
		switch (cmd)
		{
		case '1':
			bSucc = trader->qryFund();
			break;
		case '2': 
			bSucc = trader->qryOrders();
			break;
		case '3': 
			bSucc = trader->qryTrades();
			break;
		case '4': 
			bSucc = trader->qryPosition();
			break;
		case '5': 
			bSucc = trader->qrySettle();
			break;
		case '6': 
			bSucc = trader->entrustLmt(false);
			break;
		case '7': 
			bSucc = trader->entrustMkt();
			break;
		case '8': 
			bSucc = trader->cancel();
			break;
		case '9':
			bSucc = trader->entrustLmt(true);
			break;
		case '0': break;
		default:
			cmd = 'X';
			break;
		}

		if (cmd != '0' && cmd != 'X' && bSucc)
		{
			StdUniqueLock lock(g_mtxOpt);
			g_condOpt.wait(lock);
		}
		else if(cmd == '0')
			break;
	}
	
	//exit(9);
	trader->release();
	delete trader;
	return 0;
}