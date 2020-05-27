#include "../../../common/network_center.h"
#include "../../../common/message_center.h"
#include "../../../common/network_define.h"
#include "../../../common/command_line.h"
#include "../../../common/log.h"

int main(int argc, char **argv)
{
	{
		network::CommandLineParse cmd_line;
		cmd_line.Parse(argc, argv);

		mylog::SimpleLog log(mylog::EnumLogLevel::E_DEB_LV);
		network::MessageCenter msg_center(network::EnumAppProto::ENUM_BUFF);
		network::NetWorkCenter net;

		g_network_center->Init((int)network::EnumPoller::SELECT_POLLER);
		int sock = g_network_center->CreateTcpServer(cmd_line.Ip(), cmd_line.Port());
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

