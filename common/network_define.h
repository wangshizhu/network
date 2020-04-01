#ifndef NETWORK_DEFINE_H_
#define NETWORK_DEFINE_H_

namespace network
{
	typedef uint16	MessageID;
	typedef uint32	MessageLength;

	#define MESSAGE_ID_SIZE	sizeof(network::MessageID)
	#define MESSAGE_LENGTH_SIZE	sizeof(network::MessageLength)

	#define INVALID -1
	#define MESSAGE_MEMORY_DEF_SIZE 256
	#define PACKET_MAX_SIZE_TCP	1460
	#define PACKET_MAX_SIZE_UDP	1472

#define SAFE_RELEASE(i)										\
	if (i)													\
		{													\
			delete i;										\
			i = NULL;										\
		}

#define SAFE_RELEASE_ARRAY(i)								\
	if (i)													\
		{													\
			delete[] i;										\
			i = NULL;										\
		}

	enum class EnumPoller
	{
		SELECT_POLLER = 0,
		POLL_POLLER,
		EPOLL_POLLER,
	};

	enum class EnumIpProto
	{
		ENUM_TCP = 0,
		ENUM_UDP
	};

}
#endif