#ifndef PACKET_READER_H_
#define PACKET_READER_H_

#include "platform.h"
#include "socket_file_discriptor.h"

namespace network
{
	using WeakSockType = std::weak_ptr<SocketWrapper>;
	using SharedSockType = std::shared_ptr<SocketWrapper>;
	class PacketReader
	{
	public:
		PacketReader(SharedSockType sock);

	public:
		int RecvMsg();

	private:
		unsigned short cur_msg_id_;
		unsigned int cur_msg_body_len_;
		char* data_;
		WeakSockType sock_;
	};
}

#endif

