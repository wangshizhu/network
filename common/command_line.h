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

		const int IOModel();

		const char* DestIp();
		const u_int16_t DestPort();

		int ListenBacklog()const;

		bool NoAccept()const;

		bool ReuseAddr()const;

		bool SkipEOF()const;

		int AcceptSleepTime()const;

		int HandleSleepTime()const;

		int SendByteNum()const;

		bool IgnoreRST()const;

		int Linger()const;

	private:
		cmdline::parser parser_;
	};
}

#define g_CmdLine network::CommandLineParse::GetInstancePtr()

#endif // !COMMAND_LINE_H_
