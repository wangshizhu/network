#include "player_manager.h"

class MsgBase;
network::MessageHandlerMgr<MsgBase, std::function<void(MsgBase*)>> msg_mgr;

PlayerManager::PlayerManager()
{
	msg_mgr.RegisterHandler<Msg1, std::function<void(MsgBase*)>>(1, std::bind(&PlayerManager::Msg1Handler, this, std::placeholders::_1));
}

void PlayerManager::Msg1Handler(MsgBase* msg)
{
	std::cout << "Msg1Handler" << std::endl;
}