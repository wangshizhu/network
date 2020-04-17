#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "log.h"
#include "session.h"
#include "network_define.h"

namespace network
{
	class Session;
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
		virtual ~ListenTcpInputHandler();
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
		virtual EnumRecvState CatchSockError() = 0;

	};

	class TcpPacketInputHandler : public PacketInputHandler
	{
	public:
		TcpPacketInputHandler(SharedSockType sock, SharedSessionType session);
		virtual ~TcpPacketInputHandler();
		virtual int HandleInput(int fd) override;

	protected:
		EnumRecvState CatchSockError() override;

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
		virtual void HandleOutput(int fd) = 0;
	};

	class PacketOutputHandler : public OutputHandler
	{
	public:
		virtual ~PacketOutputHandler() {};
		virtual void HandleOutput(int fd) = 0;
		virtual EnumReason CatchSockError() = 0;
	};

	class TcpPacketOutputHandler : public PacketOutputHandler
	{
	public:
		TcpPacketOutputHandler(SharedSessionType session,SharedSockType sock);
		virtual ~TcpPacketOutputHandler() {};
		virtual void HandleOutput(int fd) override;
		virtual EnumReason CatchSockError() override;

	private:
		void OnGetError(int fd);

	private:
		WeakSockType accepted_sock_;
		WeakSessionType session_;
	};

	using SharedInputHandlerType = std::shared_ptr<InputHandler>;
	using SharedOutputHandlerType = std::shared_ptr<OutputHandler>;
	using WeakInputHandlerType = std::weak_ptr<InputHandler>;
	using WeakOutputHandlerType = std::weak_ptr<OutputHandler>;
	using InputMapType = std::map<int, WeakInputHandlerType>;
	using OutputMapType = std::map<int, WeakOutputHandlerType>;
	using SharedListenedInputType = std::shared_ptr<ListenInputHandler>;

}

#endif // !INTERFACE_H_
