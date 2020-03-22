#ifndef LOG_H_
#define LOG_H_

#include "singleton.h"
#include "platform.h"

namespace mylog
{
	class SimpleLog : public Singleton<SimpleLog>
	{
	public:
		SimpleLog();
		
	public:
		void DebugInfo(const char* func_name,const char* file_name,int line_num,const char* msg) const;

	private:
		short log_level_;
	};
}

#define DEBUG_INFO(m) mylog::SimpleLog::GetInstancePtr()->DebugInfo(__FUNCTION__,__FILE__,__LINE__,m);

#endif // !LOG_H_