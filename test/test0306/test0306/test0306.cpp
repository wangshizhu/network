// test0306.cpp : 定义控制台应用程序的入口点。
//


#include "../../../common/network_center.h"

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
		network::NetWorkCenter net;
		int sock = net.CreateTcpServerSocket("127.0.0.1", 5700);
		if (sock == 0)
		{
			return 0;
		}

		int conn_fd;
		struct sockaddr_in client_sock_addr;
		memset(&client_sock_addr, 0, sizeof(client_sock_addr));
		socklen_t cli_addr_len = sizeof(client_sock_addr);
		while (true)
		{
			conn_fd = ::accept(sock, (struct sockaddr *) &client_sock_addr, &cli_addr_len);
			if (conn_fd == -1)
			{
				continue;
			}

			// !TODO
			ReadData(conn_fd);

			CLOSE_SOCKET(conn_fd);
#if GENERAL_PLATFORM == PLATFORM_WIN32
			WSACleanup();
#endif
		}
	}
	return 0;
}
