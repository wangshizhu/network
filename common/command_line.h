#ifndef COMMAND_LINE_H_
#define COMMAND_LINE_H_

#include "platform.h"
#include "singleton.h"
#include "../lib/cmdline.h"

namespace network
{
	class CommandLineParse : public Singleton<CommandLineParse>
	{
	public:
		CommandLineParse();

	public:
		void Parse(int argc, char **argv);
		const char* Ip();
		const u_int16_t Port();

	private:
		cmdline::parser parser_;
	};
}

#define g_CmdLine network::CommandLineParse::GetInstancePtr()

#endif // !COMMAND_LINE_H_
