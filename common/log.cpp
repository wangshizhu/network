#include "log.h"

namespace mylog
{
	SimpleLog::SimpleLog():log_level_(0)
	{
	}

	void SimpleLog::DebugInfo(const char* func_name, const char* file_name, int line_num, const char* msg) const
	{
		std::cout << "file:" << file_name <<",function:"<<func_name<< ",line_num:" << line_num << ",msg:" << msg << std::endl;
	}
}