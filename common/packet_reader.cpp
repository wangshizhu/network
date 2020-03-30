#include "packet_reader.h"
#include "network_define.h"

namespace network
{
	PacketReader::PacketReader(SharedSockType sock):cur_msg_id_(0), cur_msg_body_len_(0), data_(nullptr), sock_(sock)
	{
	}
	int PacketReader::RecvMsg()
	{
		auto sock = sock_.lock();
		if (sock == nullptr)
		{
			return INVALID;
		}

		char* data = new char[1023];
		int len = sock->recv(data, 1023);

		std::cout << data << std::endl;

		delete[]data;

		return 0;
	}
}