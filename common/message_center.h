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

	class MessageCenter : public Singleton<MessageCenter>
	{
	public:

		MessageCenter(EnumAppProto proto) :Singleton<MessageCenter>(),
			proto_(proto),handler_mgr_(nullptr)
		{
			if (proto_ == EnumAppProto::ENUM_BUFF)
			{
				handler_mgr_ = (void*)(new MessageHandlerMgr<MsgBase, HandlerBuffFunType>);
			}
			else if (proto_ == EnumAppProto::ENUM_MSGPACK)
			{
				handler_mgr_ = (void*)(new MessageHandlerMgr<MsgBaseEx, HandlerMsgPackFunType>);
			}
		}

		~MessageCenter() 
		{
			if (proto_ == EnumAppProto::ENUM_BUFF)
			{
				MessageHandlerMgr<MsgBase, HandlerBuffFunType>* tmp = (MessageHandlerMgr<MsgBase, HandlerBuffFunType>*)handler_mgr_;
				SAFE_RELEASE(tmp);
			}
			else if (proto_ == EnumAppProto::ENUM_MSGPACK)
			{
				MessageHandlerMgr<MsgBaseEx, HandlerMsgPackFunType>* tmp = (MessageHandlerMgr<MsgBaseEx, HandlerMsgPackFunType>*)handler_mgr_;
				SAFE_RELEASE(tmp);
			}
		}

		void HandleMsg(Session* session, const MessageID msg_id, uint8 const*const msg, const MessageLength l)
		{
			/*if (proto_ == EnumAppProto::ENUM_BUFF)
			{
				buff_proto_handler_mgr_.HandleMsg(session, msg_id, msg, l);
			}
			else if (proto_ == EnumAppProto::ENUM_MSGPACK)
			{
				msg_pack_proto_handler_mgr_.HandleMsg(session, msg_id, msg, l);
			}*/
			if (proto_ == EnumAppProto::ENUM_BUFF)
			{
				MessageHandlerMgr<MsgBase, HandlerBuffFunType>* tmp = (MessageHandlerMgr<MsgBase, HandlerBuffFunType>*)handler_mgr_;
				tmp->HandleMsg(session, msg_id, msg, l);
			}
			else if (proto_ == EnumAppProto::ENUM_MSGPACK)
			{
				MessageHandlerMgr<MsgBaseEx, HandlerMsgPackFunType>* tmp = (MessageHandlerMgr<MsgBaseEx, HandlerMsgPackFunType>*)handler_mgr_;
				tmp->HandleMsg(session, msg_id, msg, l);
			}
		}
		
		template<typename MsgBaseType>
		void DeserializationMsg(MsgBaseType* base, uint8 const*const msg, const MessageLength l)
		{
			if (proto_ == EnumAppProto::ENUM_BUFF)
			{
				DeserializationMsgByBuff((MsgBase*)base, msg, l);
			}
			else if (proto_ == EnumAppProto::ENUM_MSGPACK)
			{
				DeserializationMsgByMsgPack((MsgBaseEx*)base, msg, l);
			}
		}

		template<typename MsgBaseType>
		void HandleDone(MsgBaseType* base, const MessageLength l)
		{
			if (proto_ == EnumAppProto::ENUM_BUFF)
			{
				HandleDoneByBuff((MsgBase*)base, l);
			}
			else if (proto_ == EnumAppProto::ENUM_MSGPACK)
			{
				HandleDoneByMsgPack((MsgBaseEx*)base, l);
			}
		}

		template<typename MsgBaseType>
		void SerializationMsgToMemory(MsgBaseType const*const msg, Session* session)
		{
			if (proto_ == EnumAppProto::ENUM_BUFF)
			{
				DeserializationMsgByBuff((MsgBase*)msg, session);
			}
			else if (proto_ == EnumAppProto::ENUM_MSGPACK)
			{
				SerializationMsgByMsgPack((MsgBaseEx*)msg, session);
			}
		}

		template<typename MsgSubType, typename F>
		bool RegisterHandler(const MessageID msg_id, F&& f)
		{
			if (proto_ == EnumAppProto::ENUM_BUFF)
			{
				MessageHandlerMgr<MsgBase, HandlerBuffFunType>* tmp = (MessageHandlerMgr<MsgBase, HandlerBuffFunType>*)handler_mgr_;
				tmp->RegisterHandler<MsgSubType, F>(msg_id,std::forward<F>(f));
			}
			else if (proto_ == EnumAppProto::ENUM_MSGPACK)
			{
				MessageHandlerMgr<MsgBaseEx, HandlerMsgPackFunType>* tmp = (MessageHandlerMgr<MsgBaseEx, HandlerMsgPackFunType>*)handler_mgr_;
				tmp->RegisterHandler<MsgSubType, F>(msg_id, std::forward<F>(f));
			}

			return false;
		}

	private:
		void DeserializationMsgByMsgPack(MsgBaseEx* base, uint8 const*const msg, const MessageLength l);
		void DeserializationMsgByBuff(MsgBase* base, uint8 const*const msg, const MessageLength l);
		void HandleDoneByMsgPack(MsgBaseEx* base, const MessageLength l);
		void HandleDoneByBuff(MsgBase* base, const MessageLength l);
		void SerializationMsgByMsgPack(MsgBaseEx const*const msg, Session* session);
		void SerializationMsgByBuff(MsgBase const*const msg, Session* session);

	private:
		template<typename MsgBaseType, typename Fun>
		class MessageHandler
		{
		public:
			MessageHandler(){}

			template<typename F, typename MsgType>
			MessageHandler(F&& fun, MsgType* msg) :f_(std::forward<F>(fun)), p_(msg){}

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

	private:
		EnumAppProto proto_;
		void* handler_mgr_;
		/*MessageHandlerMgr<MsgBase, HandlerBuffFunType>  buff_proto_handler_mgr_;
		MessageHandlerMgr<MsgBaseEx, HandlerMsgPackFunType> msg_pack_proto_handler_mgr_;*/
	};
}

#endif
