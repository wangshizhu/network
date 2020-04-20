#ifndef LOG_H_
#define LOG_H_

#include "singleton.h"
#include "platform.h"
#include "../lib/format.h"

namespace mylog
{
	enum EnumLogLevel
	{
		E_DEB_LV = 0,
		E_INFO_LV,
		E_WARN_LV,
		E_ERR_LV,
		E_FATAL_LV,
	};

	class SimpleLog : public Singleton<SimpleLog>
	{
	public:
		SimpleLog(EnumLogLevel lv);
		
	public:
		char* LogLevelStr(EnumLogLevel log_level)const;

		template<class... Args>
		void DebugInfo(const char* func_name,
			const char* file_name, int line_num, 
			const char* msg,Args&&... args) const
		{
			if (log_level_ > EnumLogLevel::E_DEB_LV)
			{
				return;
			}
			PrintInfo(EnumLogLevel::E_DEB_LV, func_name,file_name,line_num,msg, std::forward<decltype(args)>(args)...);
		}

		template<class... Args>
		void ErrorInfo(const char* func_name,
			const char* file_name, int line_num,
			const char* msg, Args&&... args) const
		{
			if (log_level_ > EnumLogLevel::E_ERR_LV)
			{
				return;
			}
			PrintInfo(EnumLogLevel::E_ERR_LV, func_name, file_name, line_num, msg, std::forward<decltype(args)>(args)...);
		}

		template<class... Args>
		void LogInfo(const char* func_name,
			const char* file_name, int line_num,
			const char* msg, Args&&... args) const
		{
			if (log_level_ > EnumLogLevel::E_INFO_LV)
			{
				return;
			}
			PrintInfo(EnumLogLevel::E_INFO_LV, func_name, file_name, line_num, msg, std::forward<decltype(args)>(args)...);
		}

		template<class... Args>
		void FatalInfo(const char* func_name,
			const char* file_name, int line_num,
			const char* msg, Args&&... args) const
		{
			if (log_level_ > EnumLogLevel::E_FATAL_LV)
			{
				return;
			}
			PrintInfo(EnumLogLevel::E_FATAL_LV, func_name, file_name, line_num, msg, std::forward<decltype(args)>(args)...);
		}

		template<class... Args>
		void WarnInfo(const char* func_name,
			const char* file_name, int line_num,
			const char* msg, Args&&... args) const
		{
			if (log_level_ > EnumLogLevel::E_WARN_LV)
			{
				return;
			}
			PrintInfo(EnumLogLevel::E_WARN_LV, func_name, file_name, line_num, msg, std::forward<decltype(args)>(args)...);
		}

		template<class... Args>
		void PrintInfo(EnumLogLevel log_level,const char* func_name,
			const char* file_name, int line_num,
			const char* msg, Args&&... args) const
		{
			auto&& fmt_msg = fmt::format(msg, std::forward<decltype(args)>(args)...);
			std::cout << "log:"<< LogLevelStr(log_level) <<", file: " << file_name << ", function: " << func_name << ", line_num: " << line_num << ", msg: " << fmt_msg.c_str() << std::endl;
		}

	private:
		EnumLogLevel log_level_;
	};
}

#define DEBUG_INFO(m,...) mylog::SimpleLog::GetInstancePtr()->DebugInfo(__FUNCTION__,__FILE__,__LINE__,(m),##__VA_ARGS__)
#define ERROR_INFO(m,...) mylog::SimpleLog::GetInstancePtr()->ErrorInfo(__FUNCTION__,__FILE__,__LINE__,(m),##__VA_ARGS__)
#define LOG_INFO(m,...) mylog::SimpleLog::GetInstancePtr()->LogInfo(__FUNCTION__,__FILE__,__LINE__,(m),##__VA_ARGS__)
#define FATAL_INFO(m,...) mylog::SimpleLog::GetInstancePtr()->FatalInfo(__FUNCTION__,__FILE__,__LINE__,(m),##__VA_ARGS__)
#define WARN_INFO(m,...) mylog::SimpleLog::GetInstancePtr()->WarnInfo(__FUNCTION__,__FILE__,__LINE__,(m),##__VA_ARGS__)


#endif // !LOG_H_