#ifndef PLAYER_MANAGER_H_
#define PLAYER_MANAGER_H_

#include "../../../common/session.h"
#include "../../../common/test_msg_define.h"

#include <iostream>

using SharedSessionType = std::shared_ptr<network::Session>;

class PlayerManager
{
public:
	PlayerManager();

public:
	void Msg1Handler(network::Session* session, network::MsgBase* msg);
	void Msg2Handler(network::Session* session, network::MsgBase* msg);
	void MsgS2C0407Handler(network::Session* session, network::MsgBase* msg);
	void MsgC2S10Handler(network::Session* session, network::MsgBaseEx* msg);

};

#define SEND_MSG(session,msg) msg.msg_len = sizeof(msg); \
session->WriteMsg((uint8*)&msg, msg.msg_len);\

#endif
