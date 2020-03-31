#ifndef MESSAGE_MEMORY_H_
#define MESSAGE_MEMORY_H_

#include "platform.h"
#include "network_define.h"

namespace network
{
	class MessageMemory
	{
	public:
		MessageMemory() :cur_msg_id_(0), cur_msg_body_len_(0), write_pos_(0), read_pos_(0)
		{
			data_.clear();
		}

		virtual ~MessageMemory()
		{
		}

	public:

	private:
		MessageID cur_msg_id_;
		MessageLength cur_msg_body_len_;
		uint32 write_pos_;
		uint32 read_pos_;
		std::vector<char> data_;

	};
}

#endif
