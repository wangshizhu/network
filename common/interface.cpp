#include "interface.h"
#include "network_define.h"

namespace network
{
	ListenTcpInputHandler::ListenTcpInputHandler(SharedSockType sock):listened_sock_(sock)
	{
	}

	int ListenTcpInputHandler::HandleInput(int fd)
	{
		auto sock = listened_sock_.lock();
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
				SharedSessionType session = std::make_shared<Session>();
				if (!session->Init(new_sock, (short)EnumIpProto::ENUM_TCP))
				{
					ERROR_INFO("session initial faild");
					return 0;
				}

				NetWorkCenter::GetInstancePtr()->RegisterSession(new_sock->GetSocket(),session);
			}
		}

		return 0;
	}

	TcpPacketInputHandler::TcpPacketInputHandler(SharedSockType sock) : accepted_sock_(sock)
	{
	}

	int TcpPacketInputHandler::HandleInput(int fd)
	{
		auto sock = accepted_sock_.lock();
		if (sock == nullptr)
		{
			return INVALID;
		}

		char* data = new char[1023];
		int len = sock->recv(data,1023);
		if (len < 0)
		{
			return INVALID;
		}
		if (len == 0)
		{
			return INVALID;
		}

		std::cout << data << std::endl;

		delete []data;

		return len;
	}
}