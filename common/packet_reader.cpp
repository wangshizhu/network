#include "packet_reader.h"
#include "network_define.h"

namespace network
{
	PacketReader::PacketReader(SharedSockType sock):MessageMemory(),sock_(sock)
	{
	}

	int PacketReader::RecvMsg(int max_recv_size)
	{
		auto sock = sock_.lock();
		if (sock == nullptr)
		{
			return INVALID;
		}

		uint8* data = new uint8[max_recv_size];
		int len = sock->Recv(data, max_recv_size);
		if (len <= 0)
		{
			SAFE_RELEASE_ARRAY(data);
			return len;
		}

		WriteData(data, len);

		SAFE_RELEASE_ARRAY(data);

		return len;
	}

	EnumReason PacketReader::ProcessMsg()
	{
		if (read_pos_ >= write_pos_)
		{
			return EnumReason::ENUM_NO_MSG_WAITING_PROCESS;
		}

		uint32 recv_len = write_pos_ - read_pos_;
		// 消息id还没有接收完整，继续接收
		if (recv_len < MESSAGE_ID_SIZE)
		{
			return EnumReason::ENUM_WAITING_RECV;
		}

		if (GetMsgId() == 0)
		{
			memcpy((uint8*)&cur_msg_id_, &data_[read_pos_], MESSAGE_ID_SIZE);
		}

		recv_len = recv_len - MESSAGE_ID_SIZE;
		// 消息长度还没有接收完整，继续接收
		if (recv_len < MESSAGE_LENGTH_SIZE)
		{
			return EnumReason::ENUM_WAITING_RECV;
		}

		if (GetMsgLength() == 0)
		{
			memcpy((uint8*)&cur_msg_len_, &data_[read_pos_+ MESSAGE_ID_SIZE], MESSAGE_LENGTH_SIZE);
		}

		recv_len = recv_len - MESSAGE_LENGTH_SIZE;
		// 消息体还没有接收完整，继续接收
		if (recv_len < GetMsgLength())
		{
			return EnumReason::ENUM_WAITING_RECV;
		}

		return EnumReason::ENUM_SUCCESS;
	}

	void PacketReader::ProcessMsgDone()
	{
		read_pos_ += GetMsgLength() + MESSAGE_HEAD_SIZE;
		cur_msg_id_ = 0;
		cur_msg_len_ = 0;
		if (read_pos_ == write_pos_)
		{
			memset(&data_[0], 0, read_pos_);
			read_pos_ = 0;
			write_pos_ = 0;
		}
	}
}