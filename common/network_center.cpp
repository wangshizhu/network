#include "network_center.h"
#include "socket_file_discriptor.h"

int network::NetWorkCenter::CreateTcpServerSocket(const char* ip, short port)
{
#if GENERAL_PLATFORM == PLATFORM_WIN32
	WSAData wsdata;
	WSAStartup(0x202, &wsdata);
#endif

	SocketWrapper sock;

	/* 创建字节流类型的IPV4 socket. */

	sock.CreateSocket(SOCK_STREAM);

	if (!sock.IsGood())
	{
		return 0;
	}

	// 设置非阻塞
	sock.SetNonBlocking(true);

	// 禁用Nagle
	sock.SetNoDelay();

	// 绑定到port和ip
	struct sockaddr_in server_sock_addr;

	// IPV4
	server_sock_addr.sin_family = AF_INET;
	// 指定端口
	server_sock_addr.sin_port = htons(port);

	IPToN(AF_INET, ip, &server_sock_addr.sin_addr);

	int bind_ok = bind(socket_, (struct sockaddr *) &server_sock_addr, sizeof(server_sock_addr));
	if (bind_ok < 0)
	{
		return 0;
	}

	int listen_ok = ::listen(socket_, LISTENQ);
	if (listen_ok < 0)
	{
		return 0;
	}

	return socket_;
}