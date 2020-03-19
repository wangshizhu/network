#ifndef SOCKET_FILE_DISCRIPTOR_H_
#define SOCKET_FILE_DISCRIPTOR_H_

#include "platform.h"

namespace network
{
	enum class IpProto
	{
		ENUM_TCP = 0,
		ENUM_UDP
	};

	class SocketWrapper
	{
	public:
		SocketWrapper();
		~SocketWrapper();
		SocketWrapper(const SocketWrapper&) = delete;
		SocketWrapper& operator=(const SocketWrapper&) = delete;
		SocketWrapper(SocketWrapper&&) = delete;
		SocketWrapper& operator=(SocketWrapper&&) = delete;

	public:
		int CreateTcpServerSock(const char* ip,short port);
		int SetNonBlocking(bool nonblocking);
		int SetNoDelay(bool nodelay = true);
		int SetLinger(uint16 onoff, uint16 linger);
		int GetSocket();

	private:
		GENERALSOCKET socket_;
	};
}

#endif // !SOCKET_FILE_DISCRIPTOR_H_
