/*!
 * \file WtPorter.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "PorterDefs.h"


#ifdef __cplusplus
extern "C"
{
#endif
	EXPORT_FLAG	void		register_evt_callback(FuncEventCallback cbEvt);

	EXPORT_FLAG	void		register_cta_callbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar, FuncSessionEvtCallback cbSessEvt, FuncStraCondTriggerCallback cbCondTrigger = NULL);

	EXPORT_FLAG	void		register_sel_callbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar, FuncSessionEvtCallback cbSessEvt);

	EXPORT_FLAG	void		register_hft_callbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraBarCallback cbBar,
								FuncHftChannelCallback cbChnl, FuncHftOrdCallback cbOrd, FuncHftTrdCallback cbTrd, FuncHftEntrustCallback cbEntrust,
								FuncStraOrdDtlCallback cbOrdDtl, FuncStraOrdQueCallback cbOrdQue, FuncStraTransCallback cbTrans, FuncSessionEvtCallback cbSessEvt, FuncHftPosCallback cbPosition);

	EXPORT_FLAG void		register_parser_callbacks(FuncParserEvtCallback cbEvt, FuncParserSubCallback cbSub);

	EXPORT_FLAG void		register_exec_callbacks(FuncExecInitCallback cbInit, FuncExecCmdCallback cbExec);

	EXPORT_FLAG void		register_ext_data_loader(FuncLoadFnlBars fnlBarLoader, FuncLoadRawBars rawBarLoader, FuncLoadAdjFactors fctLoader, FuncLoadRawTicks tickLoader);

	EXPORT_FLAG void		feed_raw_bars(VvTSBarStruct* bars, VvTUInt32 count);

	EXPORT_FLAG void		feed_raw_ticks(VvTSTickStruct* ticks, VvTUInt32 count);

	EXPORT_FLAG void		feed_adj_factors(VvTString stdCode, VvTUInt32* dates, double* factors, VvTUInt32 count);

	EXPORT_FLAG	void		init_porter(const char* logCfg, bool isFile, const char* genDir);

	EXPORT_FLAG	void		config_porter(const char* cfgfile, bool isFile);

	EXPORT_FLAG	void		run_porter(bool bAsync);

	EXPORT_FLAG	void		write_log(VvTUInt32 level, const char* message, const char* catName);

	EXPORT_FLAG	VvTString	get_version();

	EXPORT_FLAG	bool		reg_cta_factories(const char* factFolder);

	EXPORT_FLAG	bool		reg_hft_factories(const char* factFolder);

	EXPORT_FLAG	bool		reg_sel_factories(const char* factFolder);

	EXPORT_FLAG	bool		reg_exe_factories(const char* factFolder);

	EXPORT_FLAG	void		release_porter();

	EXPORT_FLAG	bool		create_ext_parser(const char* id);

	EXPORT_FLAG	bool		create_ext_executer(const char* id);

	EXPORT_FLAG	VvTString	get_raw_stdcode(const char* stdCode);

	//////////////////////////////////////////////////////////////////////////
	//CTA策略接口
#pragma region "CTA接口"
	EXPORT_FLAG	CtxHandler	create_cta_context(const char* name, int slippage = 0);

	EXPORT_FLAG	void		cta_enter_long(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag, double limitprice, double stopprice);

	EXPORT_FLAG	void		cta_exit_long(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag, double limitprice, double stopprice);

	EXPORT_FLAG	void		cta_enter_short(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag, double limitprice, double stopprice);

	EXPORT_FLAG	void		cta_exit_short(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag, double limitprice, double stopprice);

	EXPORT_FLAG	double		cta_get_position_profit(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	VvTUInt64	cta_get_detail_entertime(CtxHandler cHandle, const char* stdCode, const char* openTag);

	EXPORT_FLAG	double		cta_get_detail_cost(CtxHandler cHandle, const char* stdCode, const char* openTag);

	EXPORT_FLAG	double		cta_get_detail_profit(CtxHandler cHandle, const char* stdCode, const char* openTag, int flag);

	EXPORT_FLAG	double		cta_get_position_avgpx(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	double		cta_get_position(CtxHandler cHandle, const char* stdCode, bool bOnlyValid, const char* openTag);

	EXPORT_FLAG	void		cta_set_position(CtxHandler cHandle, const char* stdCode, double qty, const char* uesrTag, double limitprice, double stopprice);

	EXPORT_FLAG	double 		cta_get_price(const char* stdCode);

	EXPORT_FLAG	double 		cta_get_day_price(const char* stdCode, int flag);

	EXPORT_FLAG	double		cta_get_fund_data(CtxHandler cHandle, int flag);

	EXPORT_FLAG	VvTUInt32 	cta_get_tdate();

	EXPORT_FLAG	VvTUInt32 	cta_get_date();

	EXPORT_FLAG	VvTUInt32 	cta_get_time();

	EXPORT_FLAG	VvTUInt32	cta_get_bars(CtxHandler cHandle, const char* stdCode, const char* period, VvTUInt32 barCnt, bool isMain, FuncGetBarsCallback cb);

	EXPORT_FLAG	VvTUInt32	cta_get_ticks(CtxHandler cHandle, const char* stdCode, VvTUInt32 tickCnt, FuncGetTicksCallback cb);

	EXPORT_FLAG	void		cta_get_all_position(CtxHandler cHandle, FuncGetPositionCallback cb);

	EXPORT_FLAG	VvTUInt64	cta_get_first_entertime(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	VvTUInt64	cta_get_last_entertime(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	VvTUInt64	cta_get_last_exittime(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	double		cta_get_last_enterprice(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	VvTString	cta_get_last_entertag(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	void		cta_log_text(CtxHandler cHandle, VvTUInt32 level, const char* message);

	EXPORT_FLAG	void		cta_save_userdata(CtxHandler cHandle, const char* key, const char* val);

	EXPORT_FLAG	VvTString	cta_load_userdata(CtxHandler cHandle, const char* key, const char* defVal);

	EXPORT_FLAG	void		cta_sub_ticks(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	void		cta_sub_bar_events(CtxHandler cHandle, const char* stdCode, const char* period);

	/*
	 *	设置图表K线
	 */
	EXPORT_FLAG void		cta_set_chart_kline(CtxHandler cHandle, const char* stdCode, const char* period);

	/*
	 *	添加信号
	 */
	EXPORT_FLAG void		cta_add_chart_mark(CtxHandler cHandle, double price, const char* icon, const char* tag);

	/*
	 *	添加指标
	 *	@idxName	指标名称
	 *	@indexType	指标类型：0-主图指标，1-副图指标
	 */
	EXPORT_FLAG void		cta_register_index(CtxHandler cHandle, const char* idxName, VvTUInt32 indexType);

	/*
	 *	添加指标线
	 *	@idxName	指标名称
	 *	@lineName	线条名称
	 *	@lineType	线性，0-曲线
	 */
	EXPORT_FLAG bool		cta_register_index_line(CtxHandler cHandle, const char* idxName, const char* lineName, VvTUInt32 lineType);

	/*
	 *	添加基准线
	 *	@idxName	指标名称
	 *	@lineName	线条名称
	 *	@val		数值
	 */
	EXPORT_FLAG bool		cta_add_index_baseline(CtxHandler cHandle, const char* idxName, const char* lineName, double val);

	/*
	 *	设置指标值
	 *	@idxName	指标名称
	 *	@lineName	线条名称
	 *	@val		指标值
	 */
	EXPORT_FLAG bool		cta_set_index_value(CtxHandler cHandle, const char* idxName, const char* lineName, double val);
#pragma endregion "CTA接口"

	//////////////////////////////////////////////////////////////////////////
	//选股策略接口
#pragma  region "SEL接口"
	EXPORT_FLAG	CtxHandler	create_sel_context(const char* name, uint32_t date, uint32_t time, const char* period, const char* trdtpl = "CHINA", const char* session = "TRADING", int32_t slippage = 0);

	EXPORT_FLAG	double		sel_get_position(CtxHandler cHandle, const char* stdCode, bool bOnlyValid, const char* openTag);

	EXPORT_FLAG	void		sel_set_position(CtxHandler cHandle, const char* stdCode, double qty, const char* uesrTag);

	EXPORT_FLAG	double 		sel_get_price(const char* stdCode);

	EXPORT_FLAG	VvTUInt32 	sel_get_date();

	EXPORT_FLAG	VvTUInt32 	sel_get_time();

	EXPORT_FLAG	VvTUInt32	sel_get_bars(CtxHandler cHandle, const char* stdCode, const char* period, VvTUInt32 barCnt, FuncGetBarsCallback cb);

	EXPORT_FLAG	VvTUInt32	sel_get_ticks(CtxHandler cHandle, const char* stdCode, VvTUInt32 tickCnt, FuncGetTicksCallback cb);

	EXPORT_FLAG	void		sel_get_all_position(CtxHandler cHandle, FuncGetPositionCallback cb);

	EXPORT_FLAG	void		sel_log_text(CtxHandler cHandle, VvTUInt32 level, const char* message);

	EXPORT_FLAG	void		sel_save_userdata(CtxHandler cHandle, const char* key, const char* val);

	EXPORT_FLAG	VvTString	sel_load_userdata(CtxHandler cHandle, const char* key, const char* defVal);

	EXPORT_FLAG	void		sel_sub_ticks(CtxHandler cHandle, const char* stdCode);

	//By Wesley @ 2023.05.17
	//扩展SEL的接口，主要是和CTA接口做一个同步
	EXPORT_FLAG	double		sel_get_position_profit(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	VvTUInt64	sel_get_detail_entertime(CtxHandler cHandle, const char* stdCode, const char* openTag);

	EXPORT_FLAG	double		sel_get_detail_cost(CtxHandler cHandle, const char* stdCode, const char* openTag);

	EXPORT_FLAG	double		sel_get_detail_profit(CtxHandler cHandle, const char* stdCode, const char* openTag, int flag);

	EXPORT_FLAG	double		sel_get_position_avgpx(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	double 		sel_get_day_price(const char* stdCode, int flag);

	EXPORT_FLAG	double		sel_get_fund_data(CtxHandler cHandle, int flag);

	EXPORT_FLAG	VvTUInt32 	sel_get_tdate();

	EXPORT_FLAG	VvTUInt64	sel_get_first_entertime(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	VvTUInt64	sel_get_last_entertime(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	VvTUInt64	sel_get_last_exittime(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	double		sel_get_last_enterprice(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	VvTString	sel_get_last_entertag(CtxHandler cHandle, const char* stdCode);
#pragma endregion "SEL接口"

	//////////////////////////////////////////////////////////////////////////
	//HFT策略接口
#pragma  region "HFT接口"
	EXPORT_FLAG	CtxHandler	create_hft_context(const char* name, const char* trader, bool agent, int32_t slippage = 0);

	EXPORT_FLAG	double		hft_get_position(CtxHandler cHandle, const char* stdCode, bool bOnlyValid);

	EXPORT_FLAG	double		hft_get_position_profit(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	double		hft_get_position_avgpx(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	double		hft_get_undone(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	double 		hft_get_price(const char* stdCode);

	EXPORT_FLAG	VvTUInt32 	hft_get_date();

	EXPORT_FLAG	VvTUInt32 	hft_get_time();

	EXPORT_FLAG	VvTUInt32 	hft_get_secs();

	EXPORT_FLAG	VvTUInt32	hft_get_bars(CtxHandler cHandle, const char* stdCode, const char* period, VvTUInt32 barCnt, FuncGetBarsCallback cb);

	EXPORT_FLAG	VvTUInt32	hft_get_ticks(CtxHandler cHandle, const char* stdCode, VvTUInt32 tickCnt, FuncGetTicksCallback cb);

	EXPORT_FLAG	VvTUInt32	hft_get_ordque(CtxHandler cHandle, const char* stdCode, VvTUInt32 tickCnt, FuncGetOrdQueCallback cb);

	EXPORT_FLAG	VvTUInt32	hft_get_orddtl(CtxHandler cHandle, const char* stdCode, VvTUInt32 tickCnt, FuncGetOrdDtlCallback cb);

	EXPORT_FLAG	VvTUInt32	hft_get_trans(CtxHandler cHandle, const char* stdCode, VvTUInt32 tickCnt, FuncGetTransCallback cb);

	EXPORT_FLAG	void		hft_log_text(CtxHandler cHandle, VvTUInt32 level, const char* message);

	EXPORT_FLAG	void		hft_sub_ticks(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	void		hft_sub_order_queue(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	void		hft_sub_order_detail(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	void		hft_sub_transaction(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	bool		hft_cancel(CtxHandler cHandle, VvTUInt32 localid);

	EXPORT_FLAG	VvTString	hft_cancel_all(CtxHandler cHandle, const char* stdCode, bool isBuy);

	EXPORT_FLAG	VvTString	hft_buy(CtxHandler cHandle, const char* stdCode, double price, double qty, const char* userTag, int flag);

	EXPORT_FLAG	VvTString	hft_sell(CtxHandler cHandle, const char* stdCode, double price, double qty, const char* userTag, int flag);

	EXPORT_FLAG	void		hft_save_userdata(CtxHandler cHandle, const char* key, const char* val);

	EXPORT_FLAG	VvTString	hft_load_userdata(CtxHandler cHandle, const char* key, const char* defVal);
#pragma endregion "HFT接口"

#pragma region "扩展Parser接口"
	EXPORT_FLAG	void		parser_push_quote(const char* id, VvTSTickStruct* curTick, VvTUInt32 uProcFlag);
#pragma endregion "扩展Parser接口"

#ifdef __cplusplus
}
#endif