#include "interface.h"
#include "network_center.h"

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
			DEBUG_INFO("listened_sock_ is null fd:{0}\n",fd);
			return 0;
		}

		int tick_num = 0;
		while (tick_num++ < 256)
		{
			auto new_sock = sock->accept();
			if (new_sock == nullptr) 
			{
				DEBUG_INFO("accept invalid socket tick_num:{0}",tick_num);
				break;
			}
			else
			{
				if (NetWorkCenter::GetInstancePtr()->IsExistsSession(new_sock->GetSocket()))
				{
					ERROR_INFO("session have register fd:{0}",new_sock->GetSocket());
					continue;
				}
				SharedSessionType session = std::make_shared<Session>();
				if (!session->Init(new_sock, (short)EnumIpProto::ENUM_TCP))
				{
					ERROR_INFO("session initial faild fd:{0}",new_sock->GetSocket());
					continue;
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
			if (CatchSockError() == EnumRecvState::ENUM_RECV_STATE_INTERRUPT)
			{
				OnGetError(sock->GetSocket());
			}
			return INVALID;
		}
		if (len == 0)
		{
			OnGetError(sock->GetSocket());
			return INVALID;
		}

		session->ProcessRecvMsg();

		return len;
	}

	EnumRecvState TcpPacketInputHandler::CatchSockError()
	{
		auto sock = accepted_sock_.lock();
		if (sock == nullptr)
		{
			return EnumRecvState::ENUM_RECV_STATE_INTERRUPT;
		}

		int err_no = CatchLastError();
		DEBUG_INFO("catch socket error fd:{0},err_number:{1}\n", sock->GetSocket(), err_no);

#if GENERAL_PLATFORM == UNIX_FLAVOUR_LINUX
		// 已经无数据可读了
		if (err_no == EAGAIN)						
		{
			return EnumRecvState::ENUM_RECV_STATE_BREAK;
		}
		// 连接被服务器拒绝 或者 目的地址不可到达
		if (err_no == ECONNREFUSED || err_no == EHOSTUNREACH)
		{
			return EnumRecvState::ENUM_RECV_STATE_INTERRUPT;
		}
#else
		if (err_no == WSAEWOULDBLOCK)
		{
			return EnumRecvState::ENUM_RECV_STATE_BREAK;
		}
		if (err_no == WSAECONNRESET || err_no == WSAECONNABORTED)
		{
			return EnumRecvState::ENUM_RECV_STATE_INTERRUPT;
		}
#endif
		return EnumRecvState::ENUM_RECV_STATE_CONTINUE;
	}

	void TcpPacketInputHandler::OnGetError(int fd)
	{
		NetWorkCenter::GetInstancePtr()->DeregisterSession(fd);
		NetWorkCenter::GetInstancePtr()->DeregisterFd(fd);
	}

	TcpPacketOutputHandler::TcpPacketOutputHandler(SharedSessionType session, SharedSockType sock):
		session_(session), accepted_sock_(sock)
	{
	}

	void TcpPacketOutputHandler::HandleOutput(int fd)
	{
		auto session = session_.lock();
		if (session == nullptr)
		{
			return;
		}

		auto sock = accepted_sock_.lock();
		if (sock == nullptr)
		{
			return;
		}

		EnumReason reason = session->ProcessSendMsg();
		if (reason == EnumReason::ENUM_SEND_FAILED)
		{
			if (CatchSockError() != EnumReason::ENUM_SEND_CONTINUE)
			{
				OnGetError(sock->GetSocket());
			}
			return;
		}
		if (reason == EnumReason::ENUM_INVALID_VARIABLE)
		{
			OnGetError(sock->GetSocket());
			return;
		}
	}

	EnumReason TcpPacketOutputHandler::CatchSockError()
	{
		auto sock = accepted_sock_.lock();
		if (sock == nullptr)
		{
			return EnumReason::ENUM_INVALID_VARIABLE;
		}

		int err = CatchLastError();
		DEBUG_INFO("catch socket error fd:{0},err_number:{1}\n", sock->GetSocket(), err);

#if GENERAL_PLATFORM == PLATFORM_WIN32
		switch (err)
		{
		case WSAEWOULDBLOCK: return EnumReason::ENUM_SEND_CONTINUE;
		case WSAEINTR: return EnumReason::ENUM_SEND_CONTINUE;
		case WSAECONNREFUSED: return EnumReason::ENUM_NO_SUCH_PORT;
		case WSAECONNRESET:	return EnumReason::ENUM_CLIENT_DISCONNECTED;
		case WSAECONNABORTED: return EnumReason::ENUM_CLIENT_DISCONNECTED;
		default: return EnumReason::ENUM_GENERAL_NETWORK;
		}
#else
		switch (err)
		{
		case ECONNREFUSED:	return EnumReason::ENUM_NO_SUCH_PORT;
		case EAGAIN:		return EnumReason::ENUM_SEND_CONTINUE;
		case EPIPE:			return EnumReason::ENUM_CLIENT_DISCONNECTED;
		case ECONNRESET:	return EnumReason::ENUM_CLIENT_DISCONNECTED;
		case ENOBUFS:		return EnumReason::ENUM_TRANSMIT_QUEUE_FULL;
		default:			return EnumReason::ENUM_GENERAL_NETWORK;
		}
#endif
	}

	void TcpPacketOutputHandler::OnGetError(int fd)
	{
		NetWorkCenter::GetInstancePtr()->DeregisterSession(fd);
		NetWorkCenter::GetInstancePtr()->DeregisterFd(fd);
	}
}