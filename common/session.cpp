#include "session.h"
#include "network_center.h"
#include "interface.h"
#include "log.h"

namespace network
{
	Session::Session():sock_(nullptr),proto_(INVALID),input_(nullptr)
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
		
		SharedTcpPacketInputType handler = std::make_shared<TcpPacketInputHandler>(sock);
		if (!processor->RegisterRead(sock->GetSocket(), handler))
		{
			ERROR_INFO("regist the socket that it is accepted failed ");
			return false;
		}

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
}