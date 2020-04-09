#include "message_center.h"

namespace network
{

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