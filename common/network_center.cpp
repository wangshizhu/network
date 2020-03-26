#include "network_center.h"

network::NetWorkCenter::NetWorkCenter()
{
	listened_.clear();
	listened_input_.clear();
	event_processor_.reset(nullptr);
}

network::NetWorkCenter::~NetWorkCenter()
{
	listened_.clear();
	listened_input_.clear();
}

bool network::NetWorkCenter::Init(int poller)
{
	event_processor_ = std::make_unique<EventProcessor>(poller);
	return true;
}

void network::NetWorkCenter::Run()
{
	event_processor_->ProcessEvent();
}

int network::NetWorkCenter::CreateTcpServer(const char* ip, short port)
{
	auto it = listened_.find(port);
	if (it != listened_.end())
	{
		DEBUG_INFO("the port is listened yet");

		return 0;
	}

	network::SharedSockType sock = std::make_shared<SocketWrapper>();

	/* �����ֽ������͵�IPV4 socket. */
	sock->CreateSocket(SOCK_STREAM);

	if (!sock->IsGood())
	{
		DEBUG_INFO("the created socket isnt good");
		return 0;
	}

	// ���÷�����
	sock->SetNonBlocking(true);

	// ����Nagle
	sock->SetNoDelay();

	if (sock->bind(ip, port) < 0)
	{
		DEBUG_INFO("bind faild");
		return 0;
	}

	if (sock->listen() < 0)
	{
		DEBUG_INFO("listen faild");
		return 0;
	}

	SharedListenedInputType tmp = std::make_shared<ListenTcpInputHandler>(sock);
	if (!event_processor_->RegisterRead(sock->GetSocket(), tmp))
	{
		DEBUG_INFO("regist faild");
		return 0;
	}

	listened_input_[sock->GetSocket()] = tmp;
	listened_[port] = sock;

	return sock->GetSocket();
}