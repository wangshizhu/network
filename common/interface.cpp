#include "interface.h"

namespace network
{
	ListenTcpInputHandler::ListenTcpInputHandler(SharedSockType sock):listened_sock_(sock)
	{
	}

	int ListenTcpInputHandler::HandleInput(int fd)
	{
		auto sock = SharedSockType(listened_sock_);
		if (sock == nullptr)
		{
			DEBUG_INFO("listened_sock_ is null");
			return 0;
		}

		int tick_num = 0;
		while (tick_num++ < 256)
		{
			auto new_sock = sock->accept();
			if (new_sock == nullptr) 
			{
				break;
			}
			else
			{
				DEBUG_INFO("a new connection");
			}
		}

		return 0;
	}
}