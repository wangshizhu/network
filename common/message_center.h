#ifndef MESSAGE_CENTER_H_
#define MESSAGE_CENTER_H_

#include "platform.h"
#include "network_define.h"
#include "message_manager.h"
#include "session.h"

namespace network
{
	class Session;

	template <std::size_t v>
	struct SizeT2Type {
		enum { value = v };
	};


	/*template<typename MsgBaseType, typename Fun>
	class MessageHandler
	{
	public:
		MessageHandler() {}

		template<typename F, typename MsgType>
		MessageHandler(F&& fun, MsgType* msg) :f_(std::forward<F>(fun)), p_(msg) {}

		~MessageHandler()
		{
			SAFE_RELEASE(p_);
		}

		void HandleMsg(Session* session, uint8 const*const msg, const MessageLength l)
		{
			MessageCenter::GetInstancePtr()->DeserializationMsg(p_, msg, l);

			f_(session, p_);

			MessageCenter::GetInstancePtr()->HandleDone(p_, l);
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

		template<typename MsgSubType, typename F>
		bool RegisterHandler(const MessageID msg_id, F&& f)
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

		void HandleMsg(Session* session, const MessageID msg_id, uint8 const*const msg, const MessageLength l)
		{
			if (msg_.find(msg_id) == msg_.end())
			{
				ERROR_INFO("dont find the target msg,msg_id:{0}", msg_id);
				return;
			}
			msg_[msg_id]->HandleMsg(session, msg, l);
		}

	private:
		std::map<int, MessageHandler<MsgBaseType, Fun>*> msg_;
	};*/

	class MessageProtoBase
	{
	public:
		MessageProtoBase(EnumAppProto proto);
		virtual ~MessageProtoBase();

		virtual void HandleMsg(Session* session, const MessageID msg_id, uint8 const*const msg, const MessageLength l) = 0;

		template<typename MsgSubType, typename F,typename ProtoType,std::size_t N>
		bool RegisterHandler(const MessageID msg_id, F&& f,ProtoType (&)[N])
		{
			return RegisterHandler<MsgSubType, F>(msg_id, std::forward<F>(f), SizeT2Type<N>());
		}

		template<typename MsgSubType, typename F>
		bool RegisterHandler(const MessageID msg_id, F&& f, SizeT2Type<BUFF_PROTO_SIZE>)
		{
			MessageHandlerMgr<BuffMsgBase, HandlerBuffFunType>* p = (MessageHandlerMgr<BuffMsgBase, HandlerBuffFunType>*)handler_mgr_;
			return p->RegisterHandler<MsgSubType, F>(msg_id, std::forward<F>(f));
		}

		template<typename MsgSubType, typename F>
		bool RegisterHandler(const MessageID msg_id, F&& f, SizeT2Type<MSG_PACK_PROTO_SIZE>)
		{
			MessageHandlerMgr<MsgPackMsgBase, HandlerMsgPackFunType>* p = (MessageHandlerMgr<MsgPackMsgBase, HandlerMsgPackFunType>*)handler_mgr_;
			return p->RegisterHandler<MsgSubType, F>(msg_id, std::forward<F>(f));
		}

		template<typename MsgBaseType>
		void DeserializationMsg(MsgBaseType* base, uint8 const*const msg, const MessageLength l)
		{
			DeserializationMsg((void*)base, msg, l);
		}

		virtual void DeserializationMsg(void* base, uint8 const*const msg, const MessageLength l) = 0;

		template<typename MsgBaseType>
		void HandleDone(MsgBaseType* base, const MessageLength l)
		{
			HandleDone((void*)base,l);
		}

		virtual void HandleDone(void* base, const MessageLength l) = 0;

		template<typename MsgBaseType>
		void SerializationMsgToMemory(MsgBaseType const*const msg, Session* session)
		{
			SerializationMsgToMemory((void const*const)msg,session);
		}

		virtual void SerializationMsgToMemory(void const*const msg, Session* session) = 0;

	protected:
		void* handler_mgr_;

	private:
		EnumAppProto proto_;
	};


	class MessageBuffProto : public MessageProtoBase
	{
	public:
		MessageBuffProto(EnumAppProto proto);
		~MessageBuffProto();

		void HandleMsg(Session* session, const MessageID msg_id, uint8 const*const msg, const MessageLength l) override;
		void DeserializationMsg(void* base, uint8 const*const msg, const MessageLength l) override;
		void HandleDone(void* base, const MessageLength l);
		void SerializationMsgToMemory(void const*const msg, Session* session) override;
	};

	class MessageMsgPackProto : public MessageProtoBase
	{
	public:
		MessageMsgPackProto(EnumAppProto proto);
		~MessageMsgPackProto();

		void HandleMsg(Session* session, const MessageID msg_id, uint8 const*const msg, const MessageLength l) override;
		void DeserializationMsg(void* base, uint8 const*const msg, const MessageLength l) override;
		void HandleDone(void* base, const MessageLength l) override;
		void SerializationMsgToMemory(void const*const msg, Session* session) override;
	};


	class MessageCenter : public Singleton<MessageCenter>
	{
	public:

		MessageCenter(EnumAppProto proto);
		~MessageCenter();

		template<typename... Args>
		void HandleMsg(Args&&... args)
		{
			msg_proto_->HandleMsg(std::forward<decltype(args)>(args)...);
		}

		template<typename... Args>
		void DeserializationMsg(Args&&... args)
		{
			msg_proto_->DeserializationMsg(std::forward<decltype(args)>(args)...);
		}

		template<typename... Args>
		void HandleDone(Args&&... args)
		{
			msg_proto_->HandleDone(std::forward<decltype(args)>(args)...);
		}

		template<typename... Args>
		void SerializationMsgToMemory(Args&&... args)
		{
			msg_proto_->SerializationMsgToMemory(std::forward<decltype(args)>(args)...);
		}

		template<typename MsgSubType, typename F,typename ProtoType>
		bool RegisterHandler(const MessageID msg_id, F&& f, ProtoType& proto)
		{
			return msg_proto_->RegisterHandler<MsgSubType,F>(msg_id,std::forward<F>(f), proto);
		}
	private:
		MessageProtoBase* msg_proto_;
	};
}

#endif
