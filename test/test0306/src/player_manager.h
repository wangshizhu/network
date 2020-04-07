#ifndef PLAYER_MANAGER_H_
#define PLAYER_MANAGER_H_

#include "../../../common/message_manager.h"
#include "../../../common/test_msg_define.h"

#include <iostream>

using SharedSessionType = std::shared_ptr<network::Session>;

class PlayerManager
{
public:
	PlayerManager();

public:
	void Msg1Handler(network::Session* session,MsgBase* msg);
	void Msg2Handler(network::Session* session,MsgBase* msg);

};

#endif
