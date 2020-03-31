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
		int RecvMsg(int max_recv_size);

	private:
		WeakSockType sock_;
	};
}

#endif

