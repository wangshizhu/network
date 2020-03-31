// test0306.cpp : 定义控制台应用程序的入口点。
//


#include "../../../common/network_center.h"
#include "../../../common/network_define.h"
#include "../src/player_manager.h"

network::NetWorkCenter net;
#define g_network_center network::NetWorkCenter::GetInstancePtr()

void ReadData(int sock)
{
	int n = 1024;
	char buf[1024] = { 0 };
	int times = 0;
	while (true)
	{
		std::cout << "block in read" << std::endl;
		if ((n = ::recv(sock, buf, 1024, 0)) == 0)
			return;
		++times;
		std::cout << "1k read for " << times << std::endl;
		GeneralSleep(1000);
	}
}

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
