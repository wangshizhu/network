#ifndef NETWORK_CENTER_H_
#define NETWORK_CENTER_H_

#include "platform.h"
#include "socket_file_discriptor.h"

namespace network
{
	class InputHandler
	{
	public:
		virtual ~InputHandler() {};
		virtual int HandleInput(int fd) = 0;
	};

	class OutputHandler
	{
	public:
		virtual ~OutputHandler() {};
		virtual int HandleOutput (int fd) = 0;
	};

	class NetWorkCenter
	{
	public:
		NetWorkCenter();
		~NetWorkCenter();

	public:
		int CreateTcpServerSocket(const char* ip, short port);

	private:
		std::map<int, SocketWrapper> created_;
	};
}

#endif // !NETWORK_CENTER_H_