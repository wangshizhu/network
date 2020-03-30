#ifndef SESSION_H_
#define SESSION_H_

#include "socket_file_discriptor.h"
#include "network_define.h"
#include "interface.h"
#include "packet_reader.h"

namespace network
{
	class TcpPacketInputHandler;
	using SharedSockType = std::shared_ptr<SocketWrapper>;
	using SharedTcpPacketInputType = std::shared_ptr<TcpPacketInputHandler>;
	using UniqPacketReaderType = std::unique_ptr<PacketReader>;

	class Session : public std::enable_shared_from_this<Session>
	{
	public:
		Session();
		~Session();

	public:
		bool Init(SharedSockType sock, int proto);
		const bool IsTcp() const;
		const bool IsUdp() const;
		int RecvMsg(int max_recv_size);

	private:
		SharedSockType sock_;
		short proto_;
		SharedTcpPacketInputType input_;
		UniqPacketReaderType reader_;
	};
}

#endif
