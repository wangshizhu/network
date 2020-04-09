#include "session.h"
#include "network_center.h"
#include "message_center.h"
#include "interface.h"
#include "log.h"

namespace network
{
	Session::Session():sock_(nullptr),proto_(INVALID),input_(nullptr), reader_(nullptr),
		sender_(nullptr)
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

	void Session::ProcessRecvMsg()
	{
		while (true)
		{
			EnumReason reason = reader_->ProcessMsg();
			if (reason != ENUM_SUCCESS)
			{
				return;
			}

			MessageCenter::GetInstancePtr()->HandleMsg(this,reader_->GetMsgId(), reader_->GetMsgBodyAddr(), reader_->GetMsgLength());

			reader_->ProcessMsgDone();
		}
	}

	void Session::WriteMsg(uint8 const*const msg,MessageLength len)
	{
		if (!TryToCreateOutput())
		{
			return;
		}

		TryToCreateSender();

		sender_->WriteData(msg, len);
	}

	EnumReason Session::ProcessSendMsg()
	{
		if (sender_ == nullptr)
		{
			ERROR_INFO("sender is nullptr");
			return EnumReason::ENUM_INVALID_VARIABLE;
		}

		return sender_->ProcessMsg();
	}

	bool Session::TryToCreateOutput()
	{
		if (output_ != nullptr)
		{
			return true;
		}

		auto processor = NetWorkCenter::GetInstancePtr()->GetEventProcessor();
		if (processor == nullptr)
		{
			ERROR_INFO("Please create processor first");
			return false;
		}

		SharedTcpPacketOutputType output = std::make_shared<TcpPacketOutputHandler>(shared_from_this(), sock_);
		if (!processor->RegisterWrite(sock_->GetSocket(), output))
		{
			ERROR_INFO("register write failed,socket:{0}", sock_->GetSocket());
			return false;
		}

		output_ = output;

		return true;
	}

	void Session::TryToCreateSender()
	{
		if (sender_ != nullptr)
		{
			return;
		}

		sender_ = std::make_unique<PacketSender>(sock_);
	}
}