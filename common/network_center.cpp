#include "network_center.h"

network::NetWorkCenter::NetWorkCenter()
{
	created_.clear();
}

network::NetWorkCenter::~NetWorkCenter()
{
	created_.clear();
}

int network::NetWorkCenter::CreateTcpServerSocket(const char* ip, short port)
{
	SocketWrapper sock;

	/* 创建字节流类型的IPV4 socket. */
	sock.CreateSocket(SOCK_STREAM);

	if (!sock.IsGood())
	{
		return 0;
	}

	auto it = created_.find(sock.GetSocket());
	if (it != created_.end())
	{
		return 0;
	}

	// 设置非阻塞
	sock.SetNonBlocking(true);

	// 禁用Nagle
	sock.SetNoDelay();

	if (sock.bind(ip, port) < 0)
	{
		return 0;
	}

	if (sock.listen() < 0)
	{
		return 0;
	}

	int fd = sock.GetSocket();
	created_.emplace(fd,std::move(sock));

	return fd;
}