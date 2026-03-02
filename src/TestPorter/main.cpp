#define _CRT_SECURE_NO_WARNINGS
#include "../WtPorter/WtPorter.h"
//#include "../WtExecMon/WtExecPorter.h"

#include "../Includes/VvTSStruct.h"
#include "../Share/DLLHelper.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/fmtlib.h"

void PORTER_FLAG on_init(CtxHandler ctxid)
{
	fmt::print("on_init\n");
	hft_sub_ticks(ctxid, "CFFEX.IF.HOT");
}

void PORTER_FLAG on_tick(CtxHandler ctxid, const char* stdCode, VvTSTickStruct* newTick)
{
	fmt::print("on_tick\n");
}

void PORTER_FLAG on_calc(CtxHandler ctxid, VvTUInt32 uDate, VvTUInt32 uTime)
{
	fmt::print("on_calc\n");
}

void PORTER_FLAG on_bar(CtxHandler ctxid, const char* code, const char* period, VvTSBarStruct* newBar)
{
	fmt::print("on_bar\n");
}

void PORTER_FLAG on_getbar(CtxHandler ctxid, const char* code, const char* period, VvTSBarStruct* bar, bool isLast)
{
	if (bar)
		fmt::print("on_getbar: {}\n", bar->time);
	else
		int x = 1;
}

void PORTER_FLAG on_getticks(CtxHandler cHandle, const char* code, VvTSTickStruct* tick, bool isLast)
{
	fmt::print("on_getticks\n");
}

void PORTER_FLAG on_event(VvTUInt32 evtId, VvTUInt32 curDate, VvTUInt32 curTime)
{
	fmt::print("on_event\n");
}

void PORTER_FLAG on_channel_evt(CtxHandler cHandle, const char* trader, VvTUInt32 evtid)
{
	fmt::print("on_channel_evt\n");
	double undone = hft_get_undone(cHandle, "CFFEX.IF.HOT");
}

void PORTER_FLAG on_order(CtxHandler cHandle, VvTUInt32 localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled, const char* userTag)
{

}

void PORTER_FLAG on_trade(CtxHandler cHandle, VvTUInt32 localid, const char* stdCode, bool isBuy, double vol, double price, const char* userTag)
{

}

void PORTER_FLAG on_entrust(CtxHandler cHandle, VvTUInt32 localid, const char* stdCode, bool bSuccess, const char* message, const char* userTag)
{

}

void PORTER_FLAG on_order_queue(CtxHandler cHandle, const char* stdCode, VvTSOrdQueStruct* ordQue)
{

}

void PORTER_FLAG on_order_detail(CtxHandler cHandle, const char* stdCode, VvTSOrdDtlStruct* ordDtl)
{

}

void PORTER_FLAG on_transaction(CtxHandler cHandle, const char* stdCode, VvTSTransStruct* trans)
{

}

void test_porter()
{
#ifdef _WIN32
	DLLHelper::load_library("WtPorter.dll");
#else
	DLLHelper::load_library("libWtPorter.so");
#endif
	init_porter("logcfg.json", true, "./generated");

	reg_hft_factories("./hft");

	config_porter("config.json", true);

	run_porter(true);

	fmt::print("press enter key to exit\n");
	getchar();

	release_porter();
}

int main()
{
	test_porter();
	getchar();
	return 0;
}
