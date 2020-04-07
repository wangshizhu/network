#include "packet_sender.h"

namespace network
{
	PacketSender::PacketSender(SharedSockType sock):sock_(sock)
	{
	}

	EnumReason PacketSender::ProcessMsg()
	{
		if (read_pos_ >= write_pos_)
		{
			return EnumReason::ENUM_NO_MSG_WAITING_PROCESS;
		}

		auto sock = sock_.lock();
		if (sock == nullptr)
		{
			ERROR_INFO("sock is nullptr");
			return EnumReason::ENUM_INVALID_VARIABLE;
		}

		int sent = sock->Send((uint8*)&data_[read_pos_],write_pos_ - read_pos_);
		if (sent < 0)
		{
			return EnumReason::ENUM_SEND_FAILED;
		}
		if (sent == 0)
		{
			return EnumReason::ENUM_SEND_CONTINUE;
		}

		read_pos_ += sent;

		return EnumReason::ENUM_SUCCESS;
	}
}