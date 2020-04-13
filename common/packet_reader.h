#ifndef PACKET_READER_H_
#define PACKET_READER_H_

#include "platform.h"
#include "socket_file_discriptor.h"
#include "test_msg_define.h"
#include "message_memory.h"

namespace network
{
	class PacketReader : public MessageMemory
	{
	public:
		PacketReader(SharedSockType sock);

	public:
		int RecvMsg(int max_recv_size);
		EnumReason ProcessMsg() override;
		void ProcessMsgDone();

	private:
		WeakSockType sock_;
	};
}

#endif

