#ifndef TEST_MSG_DEFINE_H_
#define TEST_MSG_DEFINE_H_

#include "platform.h"
#include "network_define.h"

struct MsgHeader
{
	MsgHeader(unsigned int id) :msg_id(htonl(id)) {}
	unsigned int msg_body_len;
	network::MessageID msg_id;
};

struct MsgBase : public MsgHeader
{
	MsgBase(uint16 id) :MsgHeader(id) {}
};

struct Msg1 : public MsgBase
{
	Msg1() :MsgBase(1) {}
	char data[128];
};

#endif // !TEST_MSG_DEFINE_H_