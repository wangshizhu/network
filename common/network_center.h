#ifndef NETWORK_CENTER_H_
#define NETWORK_CENTER_H_

#include "platform.h"
#include "log.h"
#include "socket_file_discriptor.h"
#include "event_processor.h"
#include "interface.h"
#include "session.h"


namespace network
{
	class EventProcessor;
	using SharedEventProcessorType = std::shared_ptr<EventProcessor>;
	

	class NetWorkCenter : public Singleton<NetWorkCenter>
	{
	public:
		NetWorkCenter();
		~NetWorkCenter();

	public:
		bool Init(int poller);
		void Run();
		int CreateTcpServer(const char* ip, short port);
		int CreateTcpConnectionClient2Server(const char* ip, short port);
		SharedEventProcessorType GetEventProcessor();
		void RegisterSession(int sock, SharedSessionType session);
		void DeregisterSession(int fd);
		void DeregisterFd(int fd);
		SharedSessionType GetSession(int fd)const;

	private:
		std::map<short, SharedSockType> listened_;
		std::map<int, SharedListenedInputType> listened_input_;
		SharedEventProcessorType event_processor_;
		std::map<int, SharedSessionType> session_;
	};
}

#define g_network_center network::NetWorkCenter::GetInstancePtr()

#endif // !NETWORK_CENTER_H_