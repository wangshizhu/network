#ifndef TEST_MSG_DEFINE_H_
#define TEST_MSG_DEFINE_H_

#include "platform.h"

struct MsgHeader
{
	MsgHeader(unsigned int id) :msg_id(htonl(id)) {}
	unsigned int msg_body_len;
	unsigned int msg_id;
};

struct Msg1 : public MsgHeader
{
	Msg1() :MsgHeader(1) {}
	char data[128];
};

#endif // !TEST_MSG_DEFINE_H_