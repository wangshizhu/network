#ifndef NETWORK_DEFINE_H_
#define NETWORK_DEFINE_H_

namespace network
{
	#define INVALID -1
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