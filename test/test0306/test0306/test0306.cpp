// test0306.cpp : 定义控制台应用程序的入口点。
//


#include "../../../common/network_center.h"
#include "../../../common/message_center.h"
#include "../../../common/network_define.h"
#include "../src/player_manager.h"

int main()
{
	{
		network::MessageCenter msg_center(network::EnumAppProto::ENUM_BUFF);
		network::NetWorkCenter net;
		PlayerManager mgr;

		g_network_center->Init((int)network::EnumPoller::SELECT_POLLER);
		int sock = g_network_center->CreateTcpServer("127.0.0.1", 5700);
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
