#include "player_manager.h"
#include "../../../common/log.h"
#include "../../../common/message_center.h"
#include "../../../common/network_define.h"


PlayerManager::PlayerManager()
{
	network::MessageCenter::GetInstancePtr()->RegisterHandler<Msg1, network::HandlerBuffFunType>(1, std::tr1::bind(&PlayerManager::Msg1Handler, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2), network::buff_proto);
	network::MessageCenter::GetInstancePtr()->RegisterHandler<Msg2, network::HandlerBuffFunType>(2, std::tr1::bind(&PlayerManager::Msg2Handler, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2), network::buff_proto);
	network::MessageCenter::GetInstancePtr()->RegisterHandler<MsgS2C0407, network::HandlerBuffFunType>(3, std::tr1::bind(&PlayerManager::MsgS2C0407Handler, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2), network::buff_proto);

	//network::MessageCenter::GetInstancePtr()->RegisterHandler<MsgC2S10,network::HandlerMsgPackFunType>(10, std::tr1::bind(&PlayerManager::MsgC2S10Handler, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2),network::msg_pack_proto);
}

void PlayerManager::Msg1Handler(network::Session* session, network::BuffMsgBase* msg)
{
	std::cout << "Msg1Handler" << std::endl;
}

void PlayerManager::Msg2Handler(network::Session* session, network::BuffMsgBase* msg)
{
	Msg2* p = static_cast<Msg2*>(msg);
	DEBUG_INFO("msg2,msg_id:{0},msg_len:{1},data:{2}", p->msg_id, p->msg_len, p->id);
	//GeneralSleep(501);

	MsgS2C0407 resp;
	resp.id = 512;

	network::MessageCenter::GetInstancePtr()->SerializationMsgToMemory(&resp, session);
}

void PlayerManager::MsgS2C0407Handler(network::Session* session, network::BuffMsgBase* msg)
{
	MsgS2C0407* p = static_cast<MsgS2C0407*>(msg);
	DEBUG_INFO("MsgS2C0407,msg_id:{0},msg_len:{1},data:{2}", p->msg_id, p->msg_len, p->id);
}

void PlayerManager::MsgC2S10Handler(network::Session* session, network::MsgPackMsgBase* msg)
{
	MsgC2S10* p = dynamic_cast<MsgC2S10*>(msg);
	if (p == nullptr)
	{
		ERROR_INFO("dynamic cast error");
		return;
	}
	DEBUG_INFO("MsgC2S10,msg_id:{0},id:{1},level:{2}", p->msg_id, p->id,p->level);
}