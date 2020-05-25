#ifndef COMMAND_LINE_H_
#define COMMAND_LINE_H_

#include "platform.h"
#include "singleton.h"

namespace network
{
	class CommandLineParse : public Singleton<CommandLineParse>
	{
	public:
		CommandLineParse();
		~CommandLineParse();

	public:
		void Parse(int argc, char **argv);
		const char* Ip();
		const u_int16_t Port();

	private:
		char* ip_;
		u_int16_t port_;
	};
}

#endif // !COMMAND_LINE_H_
