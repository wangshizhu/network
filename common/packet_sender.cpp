#include "packet_sender.h"
#include "command_line.h"

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

		int agr_send_num = g_CmdLine->SendByteNum();
		int send_num = write_pos_ - read_pos_;
		if (agr_send_num > send_num)
		{
			agr_send_num = send_num;
		}

		int sent = sock->Send((uint8*)&data_[read_pos_], agr_send_num > 0 ? agr_send_num : send_num);
		if (sent < 0)
		{
			return EnumReason::ENUM_SEND_FAILED;
		}
		if (sent == 0)
		{
			return EnumReason::ENUM_SEND_CONTINUE;
		}

		read_pos_ += sent;
		DEBUG_INFO("send data fd:{0},sent len:{1}\n", sock->GetSocket(),sent);

		return EnumReason::ENUM_SUCCESS;
	}
}