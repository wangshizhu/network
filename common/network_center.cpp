#include "network_center.h"

namespace network
{
	NetWorkCenter::NetWorkCenter() :event_processor_(nullptr)
	{
		listened_.clear();
		listened_input_.clear();
		session_.clear();
		LOG_INFO("create NetWorkCenter obj success");
	}

	NetWorkCenter::~NetWorkCenter()
	{
		listened_.clear();
		listened_input_.clear();
	}

	bool NetWorkCenter::Init(int poller)
	{
		if (event_processor_ != nullptr)
		{
			ERROR_INFO("Processor have created");
			return false;
		}

		event_processor_ = std::make_shared<EventProcessor>(poller);

		LOG_INFO("NetWorkCenter obj init success,poller:{0}",poller);

		return true;
	}

	void NetWorkCenter::Run()
	{
		if (event_processor_ == nullptr)
		{
			ERROR_INFO("Processor havnt been created");
			return;
		}

		event_processor_->ProcessEvent();
	}

	int NetWorkCenter::CreateTcpServer(const char* ip, short port)
	{
		if (event_processor_ == nullptr)
		{
			ERROR_INFO("Please create processor first");
			return 0;
		}

		auto it = listened_.find(port);
		if (it != listened_.end())
		{
			ERROR_INFO("the port is listened yet,port:{0}",port);

			return 0;
		}

		SharedSockType sock = std::make_shared<SocketWrapper>(ip, port);

		/* 创建字节流类型的IPV4 socket. */
		sock->CreateSocket(SOCK_STREAM);

		if (!sock->IsGood())
		{
			ERROR_INFO("the created socket isnt good");
			return 0;
		}

		// 设置非阻塞
		sock->SetNonBlocking(true);

		// 禁用Nagle
		sock->SetNoDelay();

		if (sock->Bind() < 0)
		{
			ERROR_INFO("bind faild");
			return 0;
		}

		if (sock->Listen() < 0)
		{
			ERROR_INFO("listen faild");
			return 0;
		}

		SharedListenedInputType tmp = std::make_shared<ListenTcpInputHandler>(sock);
		if (!event_processor_->RegisterRead(sock->GetSocket(), tmp))
		{
			ERROR_INFO("regist faild");
			return 0;
		}

		listened_input_[sock->GetSocket()] = tmp;
		listened_[port] = sock;

		LOG_INFO("create server of tcp success,ip:{0},port:{1}",ip, port);

		return sock->GetSocket();
	}

	int NetWorkCenter::CreateTcpConnectionClient2Server(const char* ip, short port)
	{
		if (event_processor_ == nullptr)
		{
			ERROR_INFO("Please create processor first");
			return 0;
		}

		SharedSockType sock = std::make_shared<SocketWrapper>(ip, port);

		/* 创建字节流类型的IPV4 socket. */
		sock->CreateSocket(SOCK_STREAM);

		if (!sock->IsGood())
		{
			ERROR_INFO("the created socket isnt good");
			return 0;
		}

		// 禁用Nagle
		sock->SetNoDelay();

		int ret = sock->Connect();
		if (ret < 0)
		{
			ERROR_INFO("connect failed err_no:{0}", CatchLastError());
			return 0;
		}

		// 设置非阻塞
		sock->SetNonBlocking(true);

		SharedSessionType session = std::make_shared<Session>();
		if (!session->Init(sock, (short)EnumIpProto::ENUM_TCP))
		{
			ERROR_INFO("session initial faild");
			return 0;
		}

		RegisterSession(sock->GetSocket(), session);

		LOG_INFO("create tcp connection client to server success,ip of server:{0},port:{1}", ip, port);

		return sock->GetSocket();
	}

	SharedEventProcessorType NetWorkCenter::GetEventProcessor()
	{
		return event_processor_;
	}

	void NetWorkCenter::RegisterSession(int sock, SharedSessionType session)
	{
		if (session_.find(sock) != session_.end())
		{
			ERROR_INFO("register session failed,sock:{0}",sock);
			return;
		}

		session_[sock] = session;
	}

	void NetWorkCenter::DeregisterSession(int fd)
	{
		if (session_.find(fd) == session_.end())
		{
			ERROR_INFO("deregister session failed,dont find the session");
			return;
		}

		session_.erase(fd);
	}

	void NetWorkCenter::DeregisterFd(int fd)
	{
		event_processor_->DeregisterRead(fd);
		event_processor_->DeregisterWrite(fd);
	}

	bool NetWorkCenter::IsExistsSession(int fd) const
	{
		return session_.find(fd) != session_.end();
	}

	SharedSessionType NetWorkCenter::GetSession(int fd)const
	{
		auto it = session_.find(fd);
		if (it == session_.end())
		{
			return nullptr;
		}
		return it->second;
	}
}