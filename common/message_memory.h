#ifndef MESSAGE_MEMORY_H_
#define MESSAGE_MEMORY_H_

#include "platform.h"
#include "network_define.h"

namespace network
{
	class MessageMemory
	{
	public:
		MessageMemory() :cur_msg_id_(0), cur_msg_len_(0), write_pos_(0), read_pos_(0)
		{
			data_.reserve(MESSAGE_MEMORY_DEF_SIZE);
		}

		virtual ~MessageMemory()
		{
		}

	public:
		const MessageID GetMsgId()
		{
			return cur_msg_id_;
		}

		const MessageLength GetMsgLength()
		{
			return cur_msg_len_;
		}

		uint8 const*const GetReadPos()
		{
			return (uint8*)&data_[read_pos_];
		}

		void WriteData(char const*const data, const int len)
		{
			int after_len = write_pos_ + len;
			if ((size_t)after_len > data_.capacity())
			{
				data_.reserve(after_len);
			}

			if (data_.size() < after_len)
			{
				data_.resize(after_len);
			}

			memcpy((uint8*)&data_[write_pos_], data, len);
			write_pos_ += len;
		}

	protected:
		MessageID cur_msg_id_;
		MessageLength cur_msg_len_;
		uint32 write_pos_;
		uint32 read_pos_;
		std::vector<uint8> data_;

	};
}

#endif
