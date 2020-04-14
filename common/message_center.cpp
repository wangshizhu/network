#include "message_center.h"
#include "message_manager.h"

namespace network
{
	MessageProtoBase::MessageProtoBase(EnumAppProto proto) :handler_mgr_(nullptr), proto_(proto)
	{
	}
	MessageProtoBase::~MessageProtoBase()
	{
	}

	MessageBuffProto::MessageBuffProto(EnumAppProto proto) :MessageProtoBase(proto)
	{
		handler_mgr_ = (void*)(new MessageHandlerMgr<BuffMsgBase, HandlerBuffFunType>());
	}

	MessageBuffProto::~MessageBuffProto()
	{
		MessageHandlerMgr<BuffMsgBase, HandlerBuffFunType>* p = (MessageHandlerMgr<BuffMsgBase, HandlerBuffFunType>*)handler_mgr_;
		SAFE_RELEASE(p);
	}

	void MessageBuffProto::HandleMsg(Session* session, const MessageID msg_id, uint8 const*const msg, const MessageLength l)
	{
		MessageHandlerMgr<BuffMsgBase, HandlerBuffFunType>* p = (MessageHandlerMgr<BuffMsgBase, HandlerBuffFunType>*)handler_mgr_;
		p->HandleMsg(session, msg_id, msg, l);
	}

	void MessageBuffProto::DeserializationMsg(void* base, uint8 const*const msg, const MessageLength l)
	{
		memcpy((uint8*)base, msg, l);
	}

	void MessageBuffProto::HandleDone(void* base, const MessageLength l)
	{
		memset((uint8*)base, 0, l);
	}

	void MessageBuffProto::SerializationMsgToMemory(void const*const msg, Session* session)
	{
		BuffMsgBase const*const p = (BuffMsgBase*)msg;
		MsgHeader header(p->MsgId(), p->MsgLen());

		session->WriteMsg((uint8*)&header, MESSAGE_HEAD_SIZE);
		session->WriteMsg((uint8*)msg, header.msg_len);
	}

	MessageMsgPackProto::MessageMsgPackProto(EnumAppProto proto) :MessageProtoBase(proto)
	{
		handler_mgr_ = (void*)(new MessageHandlerMgr<MsgPackMsgBase, HandlerMsgPackFunType>());
	}

	MessageMsgPackProto::~MessageMsgPackProto()
	{
		MessageHandlerMgr<MsgPackMsgBase, HandlerMsgPackFunType>* p = (MessageHandlerMgr<MsgPackMsgBase, HandlerMsgPackFunType>*)handler_mgr_;
		SAFE_RELEASE(p);
	}

	void MessageMsgPackProto::HandleMsg(Session* session, const MessageID msg_id, uint8 const*const msg, const MessageLength l)
	{
		MessageHandlerMgr<MsgPackMsgBase, HandlerMsgPackFunType>* p = (MessageHandlerMgr<MsgPackMsgBase, HandlerMsgPackFunType>*)handler_mgr_;
		p->HandleMsg(session, msg_id, msg, l);
	}

	void MessageMsgPackProto::DeserializationMsg(void* base, uint8 const*const msg, const MessageLength l)
	{
		MsgPackMsgBase* p = (MsgPackMsgBase*)base;
		msgpack::unpacked  unpack;
		msgpack::unpack(&unpack, (char*)msg, l);
		msgpack::object obj = unpack.get();
		p->Read(obj);
	}

	void MessageMsgPackProto::HandleDone(void* base, const MessageLength l)
	{
		MsgPackMsgBase* p = (MsgPackMsgBase*)base;
		// TODO: msg pack 协议处理完消息后的清理操作
	}

	void MessageMsgPackProto::SerializationMsgToMemory(void const*const msg, Session* session)
	{
		MsgPackMsgBase const*const p = (MsgPackMsgBase*)msg;

		msgpack::sbuffer buffer;
		msgpack::packer<msgpack::sbuffer> pac(&buffer);
		p->Write(pac);

		MsgHeader header(p->MsgId(), buffer.size());

		session->WriteMsg((uint8*)&header, sizeof(header));
		session->WriteMsg((uint8*)buffer.data(), header.msg_len);
	}


	MessageCenter::MessageCenter(EnumAppProto proto) :Singleton<MessageCenter>(), msg_proto_(nullptr)
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

	MessageCenter::~MessageCenter()
	{
		SAFE_RELEASE(msg_proto_);
	}
}