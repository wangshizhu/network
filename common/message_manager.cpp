#ifndef MESSAGE_MANAGER_CPP_
#define  MESSAGE_MANAGER_CPP_

#include "message_manager.h"
#include "message_center.h"

namespace network
{
	template<typename MsgBaseType, typename Fun>
	void MessageHandler<MsgBaseType, Fun>::HandleMsg(Session* session, uint8 const*const msg, const MessageLength l)
	{
		g_message_center->DeserializationMsg(p_, msg, l);

		f_(session, p_);

		g_message_center->HandleDone(p_, l);
	}
}

#endif
