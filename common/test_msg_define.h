#ifndef TEST_MSG_DEFINE_H_
#define TEST_MSG_DEFINE_H_

#include "platform.h"
#include "network_define.h"

#pragma pack(push,1)

struct MsgHeader
{
	MsgHeader(network::MessageID id) :msg_id(id), msg_len(0){}
	network::MessageID msg_id;
	network::MessageLength msg_len;
};

struct MsgBase : public MsgHeader
{
	MsgBase(network::MessageID id) :MsgHeader(id) {}
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

#pragma pack(pop)

#endif // !TEST_MSG_DEFINE_H_