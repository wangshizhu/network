#include "socket_file_discriptor.h"

network::SocketWrapper::SocketWrapper() :
#if GENERAL_PLATFORM == PLATFORM_WIN32
socket_(INVALID_SOCKET)
#else
socket_(-1)
#endif
{
}

network::SocketWrapper::~SocketWrapper()
{
}

int network::SocketWrapper::CreateTcpServerSock(const char* ip, short port)
{

#if GENERAL_PLATFORM == PLATFORM_WIN32
	WSAData wsdata;
	WSAStartup(0x202, &wsdata);
#endif

	/* 创建字节流类型的IPV4 socket. */
	socket_ = (int)::socket(AF_INET, SOCK_STREAM, 0);
	if (socket_ < 0)
	{
		return 0;
	}

	// 设置非阻塞
	SetNonBlocking(true);

	// 禁用Nagle
	SetNoDelay();

	// 绑定到port和ip
	struct sockaddr_in server_sock_addr, client_sock_addr;
	memset(&client_sock_addr, 0, sizeof(client_sock_addr));

	// IPV4
	server_sock_addr.sin_family = AF_INET;
	// 指定端口
	server_sock_addr.sin_port = htons(port);

	IPToN(AF_INET, ip, &server_sock_addr.sin_addr);

	int bind_ok = bind(socket_, (struct sockaddr *) &server_sock_addr, sizeof(server_sock_addr));
	if (bind_ok < 0 )
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

int network::SocketWrapper::SetNonBlocking(bool nonblocking)
{
#if GENERAL_PLATFORM == PLATFORM_WIN32
	u_long val = nonblocking ? 1 : 0;
	return ::ioctlsocket(socket_, FIONBIO, &val);
#else
	int val = nonblocking ? O_NONBLOCK : 0;
	return ::fcntl(socket_, F_SETFL, val);
#endif
}

int network::SocketWrapper::SetNoDelay(bool nodelay)
{
	int arg = int(nodelay);
	return setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, (char*)&arg, sizeof(int));
}

int network::SocketWrapper::SetLinger(uint16 onoff, uint16 linger)
{
	struct linger l = { 0 };
	l.l_onoff = onoff;
	l.l_linger = linger;
	return setsockopt(socket_, SOL_SOCKET, SO_LINGER, (const char *)&l, sizeof(l));
}

int network::SocketWrapper::GetSocket()
{
	return socket_;
}