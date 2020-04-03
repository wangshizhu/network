// test0306.cpp : �������̨Ӧ�ó������ڵ㡣
//


#include "../../../common/network_center.h"
#include "../../../common/network_define.h"
#include "../src/player_manager.h"

network::NetWorkCenter net;
#define g_network_center network::NetWorkCenter::GetInstancePtr()

int main()
{
	{
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
