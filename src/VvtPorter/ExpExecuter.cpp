#include "ExpExecuter.h"
#include "VvtRtRunner.h"

extern VvtRtRunner& getRunner();

void ExpExecuter::init()
{
	getRunner().executer_init(name());
}

void ExpExecuter::set_position(const vvt_hashmap<std::string, double>& targets)
{
	for(auto& v : targets)
	{
		getRunner().executer_set_position(name(), v.first.c_str(), v.second);
	}
}

void ExpExecuter::on_position_changed(const char* stdCode, double targetPos)
{
	getRunner().executer_set_position(name(), stdCode, targetPos);
}
