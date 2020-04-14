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

	template<typename MsgBaseType, typename Fun>
	MessageHandlerMgr<MsgBaseType, Fun>::MessageHandlerMgr()
	{
		msg_.clear();
	}

	template<typename MsgBaseType, typename Fun>
	MessageHandlerMgr<MsgBaseType, Fun>::~MessageHandlerMgr()
	{
		for (auto p : msg_)
		{
			SAFE_RELEASE(p.second);
		}
		msg_.clear();
	}

	template<typename MsgBaseType, typename Fun>
	template<typename MsgSubType, typename F>
	bool MessageHandlerMgr<MsgBaseType, Fun>::RegisterHandler(const MessageID msg_id, F&& f)
	{
		if (msg_.find(msg_id) != msg_.end())
		{
			return false;
		}

		MsgBaseType * p = new MsgSubType();
		MessageHandler<MsgBaseType, F> * handler = new MessageHandler<MsgBaseType, F>(std::forward<F>(f), p);
		msg_[msg_id] = handler;

		return true;
	}

	template<typename MsgBaseType, typename Fun>
	void MessageHandlerMgr<MsgBaseType, Fun>::HandleMsg(Session* session, const MessageID msg_id, uint8 const*const msg, const MessageLength l)
	{
		if (msg_.find(msg_id) == msg_.end())
		{
			ERROR_INFO("dont find the target msg,msg_id:{0}", msg_id);
			return;
		}
		msg_[msg_id]->HandleMsg(session, msg, l);
	}

}

#endif
