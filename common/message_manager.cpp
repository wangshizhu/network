#ifndef MESSAGE_MANAGER_CPP_
#define  MESSAGE_MANAGER_CPP_

#include "message_manager.h"
#include "message_center.h"

namespace network
{
	template<typename MsgBaseType, typename Fun>
	MessageHandler<MsgBaseType, Fun>::MessageHandler()
	{
	}

	template<typename MsgBaseType, typename Fun>
	template<typename F, typename MsgType>
	MessageHandler<MsgBaseType, Fun>::MessageHandler(F&& fun, MsgType* msg) :f_(std::forward<F>(fun)), p_(msg)
	{
	}

	template<typename MsgBaseType, typename Fun>
	MessageHandler<MsgBaseType, Fun>::~MessageHandler()
	{
		SAFE_RELEASE(p_);
	}

	template<typename MsgBaseType, typename Fun>
	void MessageHandler<MsgBaseType, Fun>::HandleMsg(Session* session, uint8 const*const msg, const MessageLength l)
	{
		MessageCenter::GetInstancePtr()->DeserializationMsg(p_, msg, l);

		f_(session, p_);

		MessageCenter::GetInstancePtr()->HandleDone(p_, l);
	}

}

#endif
