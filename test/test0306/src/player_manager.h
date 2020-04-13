#ifndef PLAYER_MANAGER_H_
#define PLAYER_MANAGER_H_

#include "../../../common/session.h"
#include "../../../common/test_msg_define.h"

#include <iostream>

using SharedSessionType = std::shared_ptr<network::Session>;
//using MsgPackMsgHandlerType = std::tr1::function<void(network::Session*, network::MsgPackMsgBase*)>;
//using BuffMsgHandlerType = std::tr1::function<void(network::Session*, network::BuffMsgBase*)>;

class PlayerManager
{
public:
	PlayerManager();

public:
	void Msg1Handler(network::Session* session, network::BuffMsgBase* msg);
	void Msg2Handler(network::Session* session, network::BuffMsgBase* msg);
	void MsgS2C0407Handler(network::Session* session, network::BuffMsgBase* msg);
	void MsgC2S10Handler(network::Session* session, network::MsgPackMsgBase* msg);

};

#endif
