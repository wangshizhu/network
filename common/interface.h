#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "network_center.h"
#include "log.h"
#include "session.h"

namespace network
{
	using WeakSockType = std::weak_ptr<SocketWrapper>;
	using SharedSockType = std::shared_ptr<SocketWrapper>;
	using SharedSessionType = std::shared_ptr<Session>;
	using WeakSessionType = std::weak_ptr<Session>;

	class InputHandler
	{
	public:
		virtual ~InputHandler() {};
		virtual int HandleInput(int fd) = 0;
	};

	class ListenInputHandler : public InputHandler
	{
	public:
		virtual ~ListenInputHandler() {};
		virtual int HandleInput(int fd) = 0;
	};

	class ListenTcpInputHandler : public ListenInputHandler
	{
	public:
		ListenTcpInputHandler(SharedSockType sock);
		virtual ~ListenTcpInputHandler() {};
		virtual int HandleInput(int fd);

	private:
		WeakSockType listened_sock_;
	};

	class PacketInputHandler : public InputHandler
	{
	public:
		virtual ~PacketInputHandler() {};
		virtual int HandleInput(int fd) = 0;

	protected:
		virtual void CatchSockError() = 0;

	};

	class TcpPacketInputHandler : public PacketInputHandler
	{
	public:
		TcpPacketInputHandler(SharedSockType sock, SharedSessionType session);
		virtual ~TcpPacketInputHandler();
		virtual int HandleInput(int fd);

	protected:
		void CatchSockError() override;

	private:
		void OnGetError(int fd);

	private:
		WeakSockType accepted_sock_;
		WeakSessionType session_;
	};

	class OutputHandler
	{
	public:
		virtual ~OutputHandler() {};
		virtual int HandleOutput(int fd) = 0;
	};
}

#endif // !INTERFACE_H_
