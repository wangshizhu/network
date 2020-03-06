// client.cpp : 定义控制台应用程序的入口点。
//

#include "../../../common/platform.h"

wchar_t* char2wchar(const char* cs, size_t* outlen)
{
	int len = (int)((strlen(cs) + 1) * sizeof(wchar_t));
	wchar_t* ccattr = (wchar_t *)malloc(len);
	memset(ccattr, 0, len);

	size_t slen = mbstowcs(ccattr, cs, len);

	if (outlen)
	{
		if ((size_t)-1 != slen)
			*outlen = slen;
		else
			*outlen = 0;
	}

	return ccattr;
};

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

		/* 创建字节流类型的IPV4 socket. */
		int sock = (int)::socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0) {
			return 0;
		}

		// 绑定到port和ip
		struct sockaddr_in server_sock_addr;
		memset(&server_sock_addr, 0, sizeof(server_sock_addr));
		server_sock_addr.sin_family = AF_INET;
		server_sock_addr.sin_port = htons(5700);
		struct in_addr dst;
		const char* ip = "192.168.62.79";
		int r = IPToN(AF_INET, char2wchar(ip, nullptr), &server_sock_addr.sin_addr);

		int conn_rt = ::connect(sock, (struct sockaddr*)&server_sock_addr, sizeof(server_sock_addr));
		if (conn_rt < 0)
		{
			std::cout << "连接失败! 错误代码:" << WSAGetLastError() << std::endl;
			CLOSE_SOCKET(sock);
			WSACleanup();
			return 0;
		}

		SendData(sock);

		CLOSE_SOCKET(sock);
		WSACleanup();
	}

	return 0;
}
