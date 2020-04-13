#ifndef PACKET_SENDER_H_
#define PACKET_SENDER_H_

#include "platform.h"
#include "socket_file_discriptor.h"
#include "message_memory.h"

namespace network
{

	class PacketSender : public MessageMemory
	{
	public:
		PacketSender(SharedSockType sock);

	public:
		EnumReason ProcessMsg() override;

	private:
		WeakSockType sock_;
	};
	using UniqPacketSenderType = std::unique_ptr<PacketSender>;
}

#endif
