#ifndef TEST_MSG_DEFINE_H_
#define TEST_MSG_DEFINE_H_

#include "platform.h"
#include "network_define.h"

#pragma pack(push,1)

struct MsgBase : public network::MsgHeader
{
	MsgBase(network::MessageID id) : network::MsgHeader(id) {}
};

struct Msg1 : public MsgBase
{
	Msg1() :MsgBase(1) {}
	char data[128];
};

struct Msg2 : public MsgBase
{
	Msg2() :MsgBase(2), id(0){}
	int id;
};

struct MsgS2C0407 : public MsgBase
{
	MsgS2C0407() :MsgBase(3), id(0) {}
	int id;
};

#pragma pack(pop)

struct MsgBaseEx
{
	MsgBaseEx(network::MessageID id)
	{
		msg_id = id;
	}
	virtual ~MsgBaseEx() {}
	virtual void Write(msgpack::packer<msgpack::sbuffer>& pack)const {}
	virtual void Read(msgpack::object& obj) {};

	network::MessageID MsgId()const { return msg_id; }
	
	network::MessageID msg_id;
};

struct MsgC2S10 : public MsgBaseEx
{
	MsgC2S10() :MsgBaseEx(10)
	{
	}

	virtual void Write(msgpack::packer<msgpack::sbuffer>& pack)const override
	{
		WRITE_MAP(pack, 3);
		WRITE_KEY_VALUE(pack, NAME_TO_STR(msg_id), msg_id);
		WRITE_KEY_VALUE(pack, NAME_TO_STR(id), id);
		WRITE_KEY_VALUE(pack, NAME_TO_STR(level), level);
	}

	virtual void Read(msgpack::object& obj)
	{
		READ_MAP_DATA(obj, NAME_TO_STR(msg_id), msg_id);
		READ_MAP_DATA(obj, NAME_TO_STR(id), id);
		READ_MAP_DATA(obj, NAME_TO_STR(level), level);
	}

	int id;
	int level;
};

#endif // !TEST_MSG_DEFINE_H_