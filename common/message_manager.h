#ifndef MESSAGE_MANAGER_H_
#define MESSAGE_MANAGER_H_

//#include "platform.h"
//#include "singleton.h"
//#include "network_define.h"
//#include "log.h"
//#include "session.h"
//#include "message_center.h"

/*namespace network
{
	class Session;


	template<typename MsgBaseType, typename Fun>
	class MessageHandler
	{
	public:
		MessageHandler()
		{
		}

		template<typename F,typename MsgType>
		MessageHandler(F&& fun, MsgType* msg) :f_(std::forward<F>(fun)), p_(msg)
		{
		}

		~MessageHandler()
		{
			SAFE_RELEASE(p_);
		}

		void HandleMsg(Session* session, uint8 const*const msg, const MessageLength l)
		{
			MessageCenter::GetInstancePtr()->DeserializationMsg(p_, msg, l);

			f_(session, p_);

			MessageCenter::GetInstancePtr()->HandleDone(p_,l);
		}

	private:
		Fun f_;
		MsgBaseType* p_;
	};

	template<typename MsgBaseType, typename Fun>
	class MessageHandlerMgr : public Singleton<MessageHandlerMgr<MsgBaseType, Fun>>
	{
	public:

		MessageHandlerMgr()
		{
			msg_.clear();
		}

		~MessageHandlerMgr()
		{
			for (auto p : msg_)
			{
				SAFE_RELEASE(p.second);
			}
			msg_.clear();
		}

		template<typename MsgSubType,typename F>
		bool RegisterHandler(const MessageID msg_id,F&& f)
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

		void HandleMsg(Session* session,const MessageID msg_id, uint8 const*const msg,const MessageLength l)
		{
			if (msg_.find(msg_id) == msg_.end())
			{
				ERROR_INFO("dont find the target msg,msg_id:{0}", msg_id);
				return;
			}
			msg_[msg_id]->HandleMsg(session,msg,l);
		}

	private:
		std::map<int, MessageHandler<MsgBaseType, Fun>*> msg_;
	};
}*/

#endif
