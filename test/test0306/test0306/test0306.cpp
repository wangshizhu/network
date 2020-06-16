// test0306.cpp : 定义控制台应用程序的入口点。
//


#include "../../../common/network_center.h"
#include "../../../common/message_center.h"
#include "../../../common/network_define.h"
#include "../../../common/log.h"
#include "../../../common/command_line.h"
#include "../src/player_manager.h"

static void sig_pipe(int signo)
{
	LOG_INFO("catch a sig:{0}", signo);
}

int main(int argc, char **argv)
{
	{
#if GENERAL_PLATFORM == UNIX_FLAVOUR_LINUX
		signal(SIGPIPE, sig_pipe);
#endif
		network::CommandLineParse cmd_line;
		cmd_line.Parse(argc, argv);

		mylog::SimpleLog log(mylog::EnumLogLevel::E_DEB_LV);

		network::MessageCenter msg_center(network::EnumAppProto::ENUM_BUFF);

		network::NetWorkCenter net;

		PlayerManager mgr;

		g_network_center->Init((int)network::EnumPoller::SELECT_POLLER);
		int sock = g_network_center->CreateTcpServer(g_CmdLine->Ip(), g_CmdLine->Port());
		if (sock == 0)
		{
			return 0;
		}

		while (true)
		{
			net.Run();
		}
	}
	return 0;
}
