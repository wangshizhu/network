#ifndef SESSION_H_
#define SESSION_H_

#include "socket_file_discriptor.h"
#include "network_define.h"
#include "interface.h"
#include "packet_reader.h"
#include "packet_sender.h"
#include "message_manager.h"

namespace network
{
	class TcpPacketInputHandler;
	class TcpPacketOutputHandler;
	using SharedTcpPacketInputType = std::shared_ptr<TcpPacketInputHandler>;
	using SharedTcpPacketOutputType = std::shared_ptr<TcpPacketOutputHandler>;

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
		void ProcessRecvMsg();
		void WriteMsg(uint8 const*const msg, MessageLength len);
		EnumReason ProcessSendMsg();

	private:
		bool TryToCreateOutput();
		void TryToCreateSender();

	private:
		SharedSockType sock_;
		short proto_;
		SharedTcpPacketInputType input_;
		SharedTcpPacketOutputType output_;
		UniqPacketReaderType reader_;
		UniqPacketSenderType sender_;
	};
}

#endif
