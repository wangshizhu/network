#ifndef NETWORK_CENTER_H_
#define NETWORK_CENTER_H_

#include "platform.h"

namespace network
{
	class NetWorkCenter
	{
	public:
		CreateTcpServerSocket(const char* ip, short port);
	};
}

#endif // !NETWORK_CENTER_H_