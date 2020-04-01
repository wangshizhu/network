#include "player_manager.h"
#include "../../../common/log.h"

struct MsgBase;
network::MessageHandlerMgr<MsgBase, std::tr1::function<void(MsgBase*)>> msg_mgr;

PlayerManager::PlayerManager()
{
	msg_mgr.RegisterHandler<Msg1, std::tr1::function<void(MsgBase*)>>(1, std::tr1::bind(&PlayerManager::Msg1Handler, this, std::tr1::placeholders::_1));
	msg_mgr.RegisterHandler<Msg2, std::tr1::function<void(MsgBase*)>>(2, std::tr1::bind(&PlayerManager::Msg2Handler, this, std::tr1::placeholders::_1));
}

void PlayerManager::Msg1Handler(MsgBase* msg)
{
	std::cout << "Msg1Handler" << std::endl;
}

void PlayerManager::Msg2Handler(MsgBase* msg)
{
	Msg2* p = static_cast<Msg2*>(msg);
	DEBUG_INFO("msg2,msg_id:{0},msg_len:{1},data:{2}", p->msg_id, p->msg_len, p->id);
}