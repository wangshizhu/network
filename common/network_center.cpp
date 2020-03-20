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

	/* �����ֽ������͵�IPV4 socket. */
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

	// ���÷�����
	sock.SetNonBlocking(true);

	// ����Nagle
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