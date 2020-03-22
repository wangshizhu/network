#ifndef NETWORK_CENTER_H_
#define NETWORK_CENTER_H_

#include "platform.h"
#include "log.h"
#include "socket_file_discriptor.h"
#include "event_processor.h"
#include "interface.h"

namespace network
{
	enum class EnumPoller
	{
		SELECT_POLLER = 0,
		POLL_POLLER,
		EPOLL_POLLER,
	};

	using SharedSockType = std::shared_ptr<SocketWrapper>;
	using SharedListenedInputType = std::shared_ptr<ListenInputHandler>;

	class NetWorkCenter
	{
	public:
		NetWorkCenter(int poller);
		~NetWorkCenter();

	public:
		void Run();

	public:
		int CreateTcpServer(const char* ip, short port);

	private:
		std::map<short, SharedSockType> listened_;
		std::map<int, SharedListenedInputType> listened_input_;
		EventProcessor event_processor_;
	};
}

#endif // !NETWORK_CENTER_H_