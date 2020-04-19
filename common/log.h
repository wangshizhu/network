#ifndef LOG_H_
#define LOG_H_

#include "singleton.h"
#include "platform.h"
#include "../lib/format.h"

namespace mylog
{
	class SimpleLog : public Singleton<SimpleLog>
	{
	public:
		SimpleLog();
		
	public:
		template<class... Args>
		void DebugInfo(const char* func_name,
			const char* file_name, int line_num, 
			const char* msg,Args&&... args) const
		{
			auto&& fmt_msg = fmt::format(msg, std::forward<decltype(args)>(args)...);
			std::cout << "log:debug, file: " << file_name << ", function: " << func_name << ", line_num: " << line_num << ", msg: " << fmt_msg.c_str() << std::endl;
		}

		template<class... Args>
		void ErrorInfo(const char* func_name,
			const char* file_name, int line_num,
			const char* msg, Args&&... args) const
		{
			auto&& fmt_msg = fmt::format(msg, std::forward<decltype(args)>(args)...);
			std::cout << "log:error, file: " << file_name << ", function: " << func_name << ", line_num: " << line_num << ", msg: " << fmt_msg.c_str() << std::endl;
		}

		template<class... Args>
		void LogInfo(const char* func_name,
			const char* file_name, int line_num,
			const char* msg, Args&&... args) const
		{
			auto&& fmt_msg = fmt::format(msg, std::forward<decltype(args)>(args)...);
			std::cout << "log:info, file: " << file_name << ", function: " << func_name << ", line_num: " << line_num << ", msg: " << fmt_msg.c_str() << std::endl;
		}

		template<class... Args>
		void FatalInfo(const char* func_name,
			const char* file_name, int line_num,
			const char* msg, Args&&... args) const
		{
			auto&& fmt_msg = fmt::format(msg, std::forward<decltype(args)>(args)...);
			std::cout << "log:fatal, file: " << file_name << ", function: " << func_name << ", line_num: " << line_num << ", msg: " << fmt_msg.c_str() << std::endl;
		}

	private:
		short log_level_;
	};
}

#define DEBUG_INFO(m,...) mylog::SimpleLog::GetInstancePtr()->DebugInfo(__FUNCTION__,__FILE__,__LINE__,(m),##__VA_ARGS__)
#define ERROR_INFO(m,...) mylog::SimpleLog::GetInstancePtr()->ErrorInfo(__FUNCTION__,__FILE__,__LINE__,(m),##__VA_ARGS__)
#define LOG_INFO(m,...) mylog::SimpleLog::GetInstancePtr()->LogInfo(__FUNCTION__,__FILE__,__LINE__,(m),##__VA_ARGS__)
#define FATAL_INFO(m,...) mylog::SimpleLog::GetInstancePtr()->FatalInfo(__FUNCTION__,__FILE__,__LINE__,(m),##__VA_ARGS__)


#endif // !LOG_H_