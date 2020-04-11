#ifndef MESSAGE_CENTER_H_
#define MESSAGE_CENTER_H_

#include "platform.h"
#include "network_define.h"
#include "message_manager.h"
#include "session.h"

namespace network
{
	class Session;
	struct MsgBase;
	struct MsgBaseEx;
	using HandlerBuffFunType = std::tr1::function<void(Session*, MsgBase*)>;
	using HandlerMsgPackFunType = std::tr1::function<void(Session*, MsgBaseEx*)>;

	template <std::size_t v>
	struct SizeT2Type {
		enum { value = v };
	};

	template<typename MsgBaseType, typename Fun>
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
	};

	class MessageProtoBase
	{
	public:
		MessageProtoBase(EnumAppProto proto) :handler_mgr_(nullptr),proto_(proto)
		{
		}
		virtual ~MessageProtoBase() {}

		virtual void HandleMsg(Session* session, const MessageID msg_id, uint8 const*const msg, const MessageLength l) = 0;

		template<typename MsgSubType, typename F,typename ProtoType,std::size_t N>
		bool RegisterHandler(const MessageID msg_id, F&& f,ProtoType (&)[N])
		{
			return RegisterHandler<MsgSubType, F>(msg_id, std::forward<F>(f), SizeT2Type<N>());
		}

		template<typename MsgSubType, typename F>
		bool RegisterHandler(const MessageID msg_id, F&& f, SizeT2Type<BUFF_PROTO_SIZE>)
		{
			MessageHandlerMgr<MsgBase, HandlerBuffFunType>* p = (MessageHandlerMgr<MsgBase, HandlerBuffFunType>*)handler_mgr_;
			return p->RegisterHandler<MsgSubType, F>(msg_id, std::forward<F>(f));
		}

		template<typename MsgSubType, typename F>
		bool RegisterHandler(const MessageID msg_id, F&& f, SizeT2Type<MSG_PACK_PROTO_SIZE>)
		{
			MessageHandlerMgr<MsgBaseEx, HandlerMsgPackFunType>* p = (MessageHandlerMgr<MsgBaseEx, HandlerMsgPackFunType>*)handler_mgr_;
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
		MessageBuffProto(EnumAppProto proto) :MessageProtoBase(proto)
		{
			handler_mgr_ = (void*)(new MessageHandlerMgr<MsgBase, HandlerBuffFunType>());
		}

		~MessageBuffProto()
		{
			MessageHandlerMgr<MsgBase, HandlerBuffFunType>* p = (MessageHandlerMgr<MsgBase, HandlerBuffFunType>*)handler_mgr_;
			SAFE_RELEASE(p);
		}

		void HandleMsg(Session* session, const MessageID msg_id, uint8 const*const msg, const MessageLength l)
		{
			MessageHandlerMgr<MsgBase, HandlerBuffFunType>* p = (MessageHandlerMgr<MsgBase, HandlerBuffFunType>*)handler_mgr_;
			p->HandleMsg(session, msg_id, msg, l);
		}

		void DeserializationMsg(void* base, uint8 const*const msg, const MessageLength l)
		{
			memcpy((uint8*)base, msg, l);
		}

		void HandleDone(void* base, const MessageLength l)
		{
			memset((uint8*)base, 0, l);
		}

		void SerializationMsgToMemory(void const*const msg, Session* session)
		{
			MsgBase const*const p = (MsgBase*)msg;
			MsgHeader header(p->MsgId(), p->MsgLen());

			session->WriteMsg((uint8*)&header, MESSAGE_HEAD_SIZE);
			session->WriteMsg((uint8*)msg, header.msg_len);
		}
	};

	class MessageMsgPackProto : public MessageProtoBase
	{
	public:
		MessageMsgPackProto(EnumAppProto proto) :MessageProtoBase(proto)
		{
			handler_mgr_ = (void*)(new MessageHandlerMgr<MsgBaseEx, HandlerMsgPackFunType>());
		}

		~MessageMsgPackProto()
		{
			MessageHandlerMgr<MsgBaseEx, HandlerMsgPackFunType>* p = (MessageHandlerMgr<MsgBaseEx, HandlerMsgPackFunType>*)handler_mgr_;
			SAFE_RELEASE(p);
		}

		void HandleMsg(Session* session, const MessageID msg_id, uint8 const*const msg, const MessageLength l)
		{
			MessageHandlerMgr<MsgBaseEx, HandlerMsgPackFunType>* p = (MessageHandlerMgr<MsgBaseEx, HandlerMsgPackFunType>*)handler_mgr_;
			p->HandleMsg(session,msg_id,msg,l);
		}

		void DeserializationMsg(void* base, uint8 const*const msg, const MessageLength l)
		{
			MsgBaseEx* p = (MsgBaseEx*)base;
			msgpack::unpacked  unpack;
			msgpack::unpack(&unpack, (char*)msg, l);
			msgpack::object obj = unpack.get();
			p->Read(obj);
		}

		void HandleDone(void* base, const MessageLength l)
		{
			MsgBaseEx* p = (MsgBaseEx*)base;
			// TODO: msg pack 协议处理完消息后的清理操作
		}

		void SerializationMsgToMemory(void const*const msg, Session* session)
		{
			MsgBaseEx const*const p = (MsgBaseEx*)msg;

			msgpack::sbuffer buffer;
			msgpack::packer<msgpack::sbuffer> pac(&buffer);
			p->Write(pac);

			MsgHeader header(p->MsgId(), buffer.size());

			session->WriteMsg((uint8*)&header, sizeof(header));
			session->WriteMsg((uint8*)buffer.data(), header.msg_len);
		}
	};

	class MessageCenter : public Singleton<MessageCenter>
	{
	public:

		MessageCenter(EnumAppProto proto) :Singleton<MessageCenter>(), msg_proto_(nullptr)
		{
			if (proto == EnumAppProto::ENUM_BUFF)
			{
				msg_proto_ = new MessageBuffProto(proto);
			}
			else if (proto == EnumAppProto::ENUM_MSGPACK)
			{
				msg_proto_ = new MessageMsgPackProto(proto);
			}
		}

		~MessageCenter() 
		{
			SAFE_RELEASE(msg_proto_);
		}

		void HandleMsg(Session* session, const MessageID msg_id, uint8 const*const msg, const MessageLength l)
		{
			msg_proto_->HandleMsg(session, msg_id, msg, l);
		}
		
		template<typename MsgBaseType>
		void DeserializationMsg(MsgBaseType* base, uint8 const*const msg, const MessageLength l)
		{
			msg_proto_->DeserializationMsg<MsgBaseType>(base, msg, l);
		}

		template<typename MsgBaseType>
		void HandleDone(MsgBaseType* base, const MessageLength l)
		{
			msg_proto_->HandleDone<MsgBaseType>(base, l);
		}

		template<typename MsgBaseType>
		void SerializationMsgToMemory(MsgBaseType const*const msg, Session* session)
		{
			msg_proto_->SerializationMsgToMemory(msg, session);
		}

		template<typename MsgSubType, typename F,typename ProtoType>
		bool RegisterHandler(const MessageID msg_id, F&& f, ProtoType& proto)
		{
			return msg_proto_->RegisterHandler<MsgSubType,F>(msg_id,std::forward<F>(f), proto);
		}

	private:
		void DeserializationMsgByMsgPack(MsgBaseEx* base, uint8 const*const msg, const MessageLength l);
		void DeserializationMsgByBuff(MsgBase* base, uint8 const*const msg, const MessageLength l);
		void HandleDoneByMsgPack(MsgBaseEx* base, const MessageLength l);
		void HandleDoneByBuff(MsgBase* base, const MessageLength l);
		void SerializationMsgByMsgPack(MsgBaseEx const*const msg, Session* session);
		void SerializationMsgByBuff(MsgBase const*const msg, Session* session);

	private:
		MessageProtoBase* msg_proto_;
	};
}

#endif
