#ifndef MESSAGE_MANAGER_H_
#define MESSAGE_MANAGER_H_

#include "platform.h"
#include "singleton.h"
#include "network_define.h"
#include "log.h"
#include "session.h"


namespace network
{
	
	class MessageCenter;
	class Session;
	struct BuffMsgBase;
	struct MsgPackMsgBase;
	using HandlerBuffFunType = std::tr1::function<void(Session*, BuffMsgBase*)>;
	using HandlerMsgPackFunType = std::tr1::function<void(Session*, MsgPackMsgBase*)>;

	template<typename MsgBaseType, typename Fun>
	class MessageHandler
	{
	public:
		MessageHandler();

		template<typename F, typename MsgType>
		MessageHandler(F&& fun, MsgType* msg);

		~MessageHandler();

		void HandleMsg(Session* session, uint8 const*const msg, const MessageLength l);

	private:
		Fun f_;
		MsgBaseType* p_;
	};


	template<typename MsgBaseType, typename Fun>
	class MessageHandlerMgr : public Singleton<MessageHandlerMgr<MsgBaseType, Fun>>
	{
	public:
		MessageHandlerMgr();
		~MessageHandlerMgr();

		template<typename MsgSubType, typename F>
		bool RegisterHandler(const MessageID msg_id, F&& f);
		void HandleMsg(Session* session, const MessageID msg_id, uint8 const*const msg, const MessageLength l);

	private:
		std::map<int, MessageHandler<MsgBaseType, Fun>*> msg_;
	};
}

#endif
