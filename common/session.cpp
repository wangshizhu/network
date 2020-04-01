#include "session.h"
#include "network_center.h"
#include "interface.h"
#include "log.h"

namespace network
{
	Session::Session():sock_(nullptr),proto_(INVALID),input_(nullptr), reader_(nullptr)
	{
		
	}

	Session::~Session()
	{
		DEBUG_INFO("Session have released");
	}

	bool Session::Init(SharedSockType sock,int proto)
	{
		auto processor = NetWorkCenter::GetInstancePtr()->GetEventProcessor();
		if (processor == nullptr)
		{
			ERROR_INFO("Please create processor first");
			return false;
		}
		
		SharedTcpPacketInputType handler = std::make_shared<TcpPacketInputHandler>(sock,shared_from_this());
		if (!processor->RegisterRead(sock->GetSocket(), handler))
		{
			ERROR_INFO("regist the socket that it is accepted failed ");
			return false;
		}

		reader_ = std::make_unique<PacketReader>(sock);
		sock_ = sock;
		proto_ = proto;
		input_ = handler;

		return true;
	}

	const bool Session::IsTcp() const
	{
		return proto_ == (short)EnumIpProto::ENUM_TCP;
	}

	const bool Session::IsUdp() const
	{
		return proto_ == (short)EnumIpProto::ENUM_UDP;
	}

	int Session::RecvMsg(int max_recv_size)
	{
		if (reader_ == nullptr)
		{
			ERROR_INFO("reader_ is nullptr");

			return INVALID;
		}

		return reader_->RecvMsg(max_recv_size);
	}

	void Session::ProcessMsg()
	{
		while (true)
		{
			uint8 const*const p = reader_->ProcessMsg();
			if (p == nullptr)
			{
				return;
			}

			g_message_mgr->HandleMsg(reader_->GetMsgId(), p, reader_->GetMsgLength());

			reader_->ProcessMsgDone();
		}
	}
}