// client.cpp : 定义控制台应用程序的入口点。
//

#include <thread>
#include "../../../common/platform.h"
#include "../../../common/test_msg_define.h"
#include "../../../common/network_define.h"
#include "../../../common/network_center.h"
#include "../../../common/message_center.h"
#include "../../../common/command_line.h"
#include "../src/player_manager.h"
#include "../../../common/log.h"

void SendData(int sock_fd)
{
	Msg2 msg;
	msg.id = 213;

	auto session = g_network_center->GetSession(sock_fd);
	if (session == nullptr)
	{
		return;
	}


	network::MessageCenter::GetInstancePtr()->SerializationMsgToMemory(&msg, session.get());
}

void Send10Msg(int sock_fd)
{
	MsgC2S10 msg;
	msg.id = 10;
	msg.level = 1;

	auto session = g_network_center->GetSession(sock_fd);
	if (session == nullptr)
	{
		return;
	}

	network::MessageCenter::GetInstancePtr()->SerializationMsgToMemory(&msg, session.get());
}

void MoreSendData(int sock_fd)
{
	while (true)
	{
		int i = 0;
		std::cout << "enter send num:";
		std::cin >> i;
		if (std::cin.fail())
		{
			return;
		}
		for (int j = 0; j < i; ++j)
		{
			SendData(sock_fd);
		}
	}
}

int main(int argc, char **argv)
{

	{
		network::CommandLineParse cmd_line;
		cmd_line.Parse(argc, argv);

		mylog::SimpleLog log(mylog::EnumLogLevel::E_DEB_LV);

		network::MessageCenter msg_center(network::EnumAppProto::ENUM_BUFF);

		network::NetWorkCenter net;

		PlayerManager mgr;

		if (!g_network_center->Init((int)network::EnumPoller::SELECT_POLLER))
		{
			return 0;
		}
		int sock = g_network_center->CreateTcpConnectionClient2Server(g_CmdLine->Ip(), g_CmdLine->Port());
		if (sock == 0)
		{
			return 0;
		}

		std::thread t(MoreSendData, sock);
		t.detach();

		while (true)
		{
			net.Run();
		}
	}

	return 0;
}
