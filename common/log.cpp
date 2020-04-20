#include "log.h"

namespace mylog
{
	SimpleLog::SimpleLog(EnumLogLevel lv):log_level_(lv)
	{
	}

	char* SimpleLog::LogLevelStr(EnumLogLevel log_level)const
	{
		switch (log_level)
		{
		case EnumLogLevel::E_DEB_LV: return "debug";
		case EnumLogLevel::E_INFO_LV: return "info";
		case EnumLogLevel::E_WARN_LV: return "warn";
		case EnumLogLevel::E_ERR_LV: return "error";
		case EnumLogLevel::E_FATAL_LV: return "fatal";
		default: return "debug";
		}
	}
}