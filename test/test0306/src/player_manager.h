#ifndef PLAYER_MANAGER_H_
#define PLAYER_MANAGER_H_

#include "../../../common/message_manager.h"
#include "../../../common/test_msg_define.h"

#include <iostream>

class PlayerManager
{
public:
	PlayerManager();

public:
	void Msg1Handler(MsgBase* msg);

};

#endif
