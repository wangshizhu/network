#ifndef SOCKET_FILE_DISCRIPTOR_H_
#define SOCKET_FILE_DISCRIPTOR_H_

#include "platform.h"
#include "log.h"
#include "address.h"

namespace network
{

	class SocketWrapper
	{
	public:
		SocketWrapper();
		~SocketWrapper();
		SocketWrapper(const SocketWrapper&) = delete;
		SocketWrapper& operator=(const SocketWrapper&) = delete;
		SocketWrapper(SocketWrapper&&) = default;
		SocketWrapper& operator=(SocketWrapper&&) = delete;

	public:
		int bind(const char* ip, short port);
		int listen();
		std::shared_ptr<SocketWrapper> accept();
		int SetNonBlocking(bool nonblocking);
		int SetNoDelay(bool nodelay = true);
		int SetLinger(uint16 onoff, uint16 linger);
		int close();
		int recv(void* gram_data, int gram_size);
		void SetSocket(GENERALSOCKET sock);
		int Send(const void * gram_data, int gram_size);
		int Connect(const char* ip, short port);

	public:
		void CreateSocket(int type);
		bool IsGood()const;
		int GetSocket();

	private:
		void InittdNetWork();

	private:
		GENERALSOCKET socket_;
		Address addr_;

	};

	using SharedSockType = std::shared_ptr<SocketWrapper>;
	using WeakSockType = std::weak_ptr<SocketWrapper>;
}

#endif // !SOCKET_FILE_DISCRIPTOR_H_
