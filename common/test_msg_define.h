#ifndef TEST_MSG_DEFINE_H_
#define TEST_MSG_DEFINE_H_

#include "platform.h"
#include "network_define.h"

#pragma pack(push,1)

struct Msg1 : public network::MsgBase
{
	Msg1() :network::MsgBase(1, sizeof(Msg1)) {}
	char data[128];
};

struct Msg2 : public network::MsgBase
{
	Msg2() :network::MsgBase(2, sizeof(Msg2)), id(0){}
	int id;
};

struct MsgS2C0407 : public network::MsgBase
{
	MsgS2C0407() :network::MsgBase(3, sizeof(MsgS2C0407)), id(0) {}
	int id;
};

#pragma pack(pop)

struct MsgC2S10 : public network::MsgBaseEx
{
	MsgC2S10() :network::MsgBaseEx(10)
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