#include "packet_reader.h"
#include "network_define.h"

namespace network
{
	PacketReader::PacketReader(SharedSockType sock):sock_(sock)
	{
	}

	int PacketReader::RecvMsg(int max_recv_size)
	{
		auto sock = sock_.lock();
		if (sock == nullptr)
		{
			return INVALID;
		}

		char* data = new char[max_recv_size];
		int len = sock->recv(data, max_recv_size);
		if (len <= 0)
		{
			SAFE_RELEASE_ARRAY(data);
			return len;
		}

		SAFE_RELEASE_ARRAY(data);

		return 0;
	}
}