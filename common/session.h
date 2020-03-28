#ifndef SESSION_H_
#define SESSION_H_

#include "socket_file_discriptor.h"
#include "network_define.h"
#include "interface.h"

namespace network
{
	class TcpPacketInputHandler;
	using SharedSockType = std::shared_ptr<SocketWrapper>;
	using SharedTcpPacketInputType = std::shared_ptr<TcpPacketInputHandler>;

	class Session
	{
	public:
		Session();
		~Session();

	public:
		bool Init(SharedSockType sock, int proto);
		const bool IsTcp() const;
		const bool IsUdp() const;

	private:
		SharedSockType sock_;
		short proto_;
		SharedTcpPacketInputType input_;
	};
}

#endif
