#ifndef MESSAGE_CENTER_H_
#define MESSAGE_CENTER_H_

#include "platform.h"
#include "network_define.h"
#include "session.h"
#include "message_manager.h"

namespace network
{
	class Session;

	template <std::size_t v>
	struct SizeT2Type {
		enum { value = v };
	};

	class MessageProtoBase
	{
	public:
		MessageProtoBase(EnumAppProto proto);
		virtual ~MessageProtoBase();

		virtual void HandleMsg(Session* session, const MessageID msg_id, uint8 const*const msg, const MessageLength l) = 0;

		template<typename MsgSubType, typename F, typename ProtoType, std::size_t N>
		bool RegisterHandler(const MessageID msg_id, F&& f, ProtoType(&)[N])
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

#include "message_manager.cpp"

#endif
