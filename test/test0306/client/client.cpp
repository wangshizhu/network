// client.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "../../../common/platform.h"

void SendData(int sock_fd)
{
	int n = 102400;
	char* data = new char[n + 1];
	data[n] = '\0';
	for (int i = 0; i < n; i++)
	{
		data[i] = 'a';
	}

	const char* p = data;
	size_t remaining = strlen(data);
	while (remaining)
	{
		int writed = send(sock_fd, p, remaining, 0);
		std::cout << "this loop send into buffer " << writed << std::endl;
		if (writed <= 0)
		{
			return;
		}
		remaining -= writed;
		p += writed;
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
		struct sockaddr_in server_sock_addr;
		memset(&server_sock_addr, 0, sizeof(server_sock_addr));
		server_sock_addr.sin_family = AF_INET;
		server_sock_addr.sin_port = htons(5700);
		struct in_addr dst;
		const char* ip = "192.168.62.79";
		int r = IPToN(AF_INET, ip, &server_sock_addr.sin_addr);

		int conn_rt = ::connect(sock, (struct sockaddr*)&server_sock_addr, sizeof(server_sock_addr));
		if (conn_rt < 0)
		{
#if GENERAL_PLATFORM == PLATFORM_WIN32
			std::cout << "����ʧ��! �������:" << WSAGetLastError() << std::endl;
#endif

			CLOSE_SOCKET(sock);

#if GENERAL_PLATFORM == PLATFORM_WIN32
			WSACleanup();
#endif
			return 0;
		}

		SendData(sock);

		CLOSE_SOCKET(sock);

#if GENERAL_PLATFORM == PLATFORM_WIN32
		WSACleanup();
#endif
	}

	return 0;
}
