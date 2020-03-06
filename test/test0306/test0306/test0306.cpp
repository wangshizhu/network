// test0306.cpp : �������̨Ӧ�ó������ڵ㡣
//


#include "../../../common/platform.h"

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
#if GENERAL_PLATFORM == PLATFORM_WIN32
		WSAData wsdata;
		WSAStartup(0x202, &wsdata);
#endif

		/* �����ֽ������͵�IPV4 socket. */
		int sock = (int)::socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0) {
			return 0;
		}

		// �󶨵�port��ip
		struct sockaddr_in serverSockAddr, client_sock_addr;

		memset(&client_sock_addr, 0, sizeof(client_sock_addr));
		// IPV4
		serverSockAddr.sin_family = AF_INET;
		// ָ���˿�
		serverSockAddr.sin_port = htons(5700);
		struct in_addr dst;
		// ͨ���ַ
		serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

		// ��IPV4��ַת����ͨ�õ�ַ��ʽ��ͬʱ���ݳ���
		if (bind(sock, (struct sockaddr *) &serverSockAddr, sizeof(serverSockAddr)) < 0)
		{
			return 0;
		}

		::listen(sock, 1024);

		int conn_fd;
		int cli_addr_len = sizeof(client_sock_addr);
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
