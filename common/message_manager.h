#ifndef MESSAGE_MANAGER_H_
#define MESSAGE_MANAGER_H_

#include "platform.h"
#include "singleton.h"
#include "network_define.h"
#include "log.h"

namespace network
{
	template<typename MsgBaseType, typename Fun>
	class MessageHandler
	{
	public:
		MessageHandler() 
		{
		}

		template<typename MsgSubType, typename F>
		MessageHandler(F&& fun, MsgSubType* msg) :f_(std::forward<F>(fun)), p_(msg)
		{
		}

		~MessageHandler()
		{
			SAFE_RELEASE(p_);
		}

		void HandleMsg(uint8 const*const msg, const MessageLength l)
		{
			memcpy((uint8*)p_, msg, l);
			f_(p_);
			/*memset((uint8*)p_, 0, l);*/
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

		template<typename MsgSubType,typename F>
		bool RegisterHandler(const MessageID msg_id,F&& f)
		{
			if (msg_.find(msg_id) != msg_.end())
			{
				return false;
			}

			msg_[msg_id] = MessageHandler<MsgBaseType, F>(std::forward<F>(f), new MsgSubType());
			
			return true;
		}

		void HandleMsg(const MessageID msg_id, uint8 const*const msg,const MessageLength l)
		{
			if (msg_.find(msg_id) == msg_.end())
			{
				ERROR_INFO("dont find the target msg,msg_id:{0}", msg_id);
				return;
			}
			msg_[msg_id].HandleMsg(msg,l);
		}

	private:
		std::map<int, MessageHandler<MsgBaseType, Fun>> msg_;
	};

}

#define g_message_mgr network::MessageHandlerMgr<MsgBase, std::tr1::function<void(MsgBase*)>>::GetInstancePtr()

#endif
