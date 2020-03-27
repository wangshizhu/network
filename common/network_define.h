#ifndef NETWORK_DEFINE_H_
#define NETWORK_DEFINE_H_

namespace network
{
	#define INVALID -1

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