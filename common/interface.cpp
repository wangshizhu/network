#include "interface.h"
#include "network_define.h"

namespace network
{
	ListenTcpInputHandler::ListenTcpInputHandler(SharedSockType sock):listened_sock_(sock)
	{
	}

	ListenTcpInputHandler::~ListenTcpInputHandler()
	{
		DEBUG_INFO("listened_sock_ have released");
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

	TcpPacketInputHandler::TcpPacketInputHandler(SharedSockType sock, SharedSessionType session) 
		: accepted_sock_(sock),session_(session)
	{
	}

	TcpPacketInputHandler::~TcpPacketInputHandler()
	{
		DEBUG_INFO("TcpPacketInputHandler have released");
	}

	int TcpPacketInputHandler::HandleInput(int fd)
	{
		auto sock = accepted_sock_.lock();
		if (sock == nullptr)
		{
			return INVALID;
		}

		auto session = session_.lock();
		if (session == nullptr)
		{
			return INVALID;
		}

		int len = session->RecvMsg(PACKET_MAX_SIZE_TCP);
		if (len < 0)
		{
			CatchSockError();
			OnGetError(sock->GetSocket());
			return INVALID;
		}
		if (len == 0)
		{
			OnGetError(sock->GetSocket());
			return INVALID;
		}

		session->ProcessMsg();

		return len;
	}

	void TcpPacketInputHandler::CatchSockError()
	{
		auto sock = accepted_sock_.lock();
		if (sock == nullptr)
		{
			return;
		}

#if GENERAL_PLATFORM == PLATFORM_WIN32
		DWORD win_err = WSAGetLastError();
#endif
#if GENERAL_PLATFORM == UNIX_FLAVOUR_LINUX
		if (errno == EAGAIN ||							// 已经无数据可读了
			errno == ECONNREFUSED ||					// 连接被服务器拒绝
			errno == EHOSTUNREACH)						// 目的地址不可到达
		{
			DEBUG_INFO("catch socket error fd:{0},err_number:{1}\n", sock->GetSocket(),errno);
			return;
		}
#else
		DEBUG_INFO("catch socket error fd:{0},err_number:{1}\n", sock->GetSocket(), win_err);

#endif
	}

	void TcpPacketInputHandler::OnGetError(int fd)
	{
		NetWorkCenter::GetInstancePtr()->DeregisterSession(fd);
		NetWorkCenter::GetInstancePtr()->DeregisterFd(fd);
	}
}