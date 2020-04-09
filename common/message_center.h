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
		MessageCenter(EnumAppProto proto);
		MessageHandlerMgr<MsgBase, HandlerBuffFunType>* BuffProtoHandlerMgr();
		MessageHandlerMgr<MsgBaseEx, HandlerMsgPackFunType>* MsgPackProtoHandlerMgr();

		void HandleMsg(Session* session, const MessageID msg_id, uint8 const*const msg, const MessageLength l);
		
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

	private:
		void DeserializationMsgByMsgPack(MsgBaseEx* base, uint8 const*const msg, const MessageLength l);

		void DeserializationMsgByBuff(MsgBase* base, uint8 const*const msg, const MessageLength l);

		void HandleDoneByMsgPack(MsgBaseEx* base, const MessageLength l);

		void HandleDoneByBuff(MsgBase* base, const MessageLength l);

		void SerializationMsgByMsgPack(MsgBaseEx const*const msg, Session* session);

		void SerializationMsgByBuff(MsgBase const*const msg, Session* session);

	private:
		EnumAppProto proto_;
		MessageHandlerMgr<MsgBase, HandlerBuffFunType>  buff_proto_handler_mgr_;
		MessageHandlerMgr<MsgBaseEx, HandlerMsgPackFunType> msg_pack_proto_handler_mgr_;
	};
}

#endif
