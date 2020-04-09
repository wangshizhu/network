#include "message_center.h"

namespace network
{
	MessageCenter::MessageCenter(EnumAppProto proto) :Singleton<MessageCenter>(), 
		proto_(proto), buff_proto_handler_mgr_(), msg_pack_proto_handler_mgr_()
	{
	}
	MessageHandlerMgr<MsgBase, HandlerBuffFunType>* MessageCenter::BuffProtoHandlerMgr()
	{
		return &buff_proto_handler_mgr_;
	}
	MessageHandlerMgr<MsgBaseEx, HandlerMsgPackFunType>* MessageCenter::MsgPackProtoHandlerMgr()
	{
		return &msg_pack_proto_handler_mgr_;
	}

	void MessageCenter::HandleMsg(Session* session, const MessageID msg_id, uint8 const*const msg, const MessageLength l)
	{
		if (proto_ == EnumAppProto::ENUM_BUFF)
		{
			buff_proto_handler_mgr_.HandleMsg(session, msg_id, msg,l);
		}
		else if (proto_ == EnumAppProto::ENUM_MSGPACK)
		{
			msg_pack_proto_handler_mgr_.HandleMsg(session, msg_id, msg, l);
		}
	}

	void MessageCenter::DeserializationMsgByMsgPack(MsgBaseEx* base, uint8 const*const msg, const MessageLength l)
	{
		msgpack::unpacked  unpack;
		msgpack::unpack(&unpack, (char*)msg, l);
		msgpack::object obj = unpack.get();
		base->Read(obj);
	}

	void MessageCenter::DeserializationMsgByBuff(MsgBase* base, uint8 const*const msg, const MessageLength l)
	{
		memcpy((uint8*)base, msg, l);
	}

	void MessageCenter::HandleDoneByMsgPack(MsgBaseEx* base, const MessageLength l)
	{
	}

	void MessageCenter::HandleDoneByBuff(MsgBase* base, const MessageLength l)
	{
		memset((uint8*)base, 0, l);
	}

	void MessageCenter::SerializationMsgByMsgPack(MsgBaseEx const*const msg, Session* session)
	{
		msgpack::sbuffer buffer;
		msgpack::packer<msgpack::sbuffer> pac(&buffer);
		msg->Write(pac);

		MsgHeader header(msg->MsgId(), buffer.size());

		session->WriteMsg((uint8*)&header, sizeof(header));
		session->WriteMsg((uint8*)buffer.data(), header.msg_len);
	}

	void MessageCenter::SerializationMsgByBuff(MsgBase const*const msg, Session* session)
	{
		MsgHeader header(msg->MsgId(), msg->MsgLen());

		session->WriteMsg((uint8*)&header, MESSAGE_HEAD_SIZE);
		session->WriteMsg((uint8*)msg, header.msg_len);
	}
}