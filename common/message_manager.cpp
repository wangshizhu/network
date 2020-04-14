#ifndef MESSAGE_MANAGER_CPP_
#define  MESSAGE_MANAGER_CPP_

#include "message_manager.h"
#include "message_center.h"

namespace network
{
	template<typename MsgBaseType, typename Fun>
	void MessageHandler<MsgBaseType, Fun>::HandleMsg(Session* session, uint8 const*const msg, const MessageLength l)
	{
		MessageCenter::GetInstancePtr()->DeserializationMsg(p_, msg, l);

		f_(session, p_);

		MessageCenter::GetInstancePtr()->HandleDone(p_, l);
	}
}

#endif
