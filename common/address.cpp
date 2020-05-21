#include "address.h"

namespace network
{
	Address::Address():ip_(0),port_(0)
	{
	}

	Address::Address(const char* ip, u_int16_t port)
	{
		Address::String2Ip(ip, ip_);
		port_ = htons(port);
	}

	int Address::String2Ip(const char * ip, u_int32_t & address)
	{
		u_int32_t	trial;

#if GENERAL_PLATFORM != PLATFORM_WIN32
		if (inet_aton(string, (struct in_addr*)&trial) != 0)
#else
		if ((trial = inet_addr(ip)) != INADDR_NONE)
#endif
		{
			address = trial;
			return 0;
		}

		struct hostent * hosts = gethostbyname(ip);
		if (hosts != NULL)
		{
			address = *(u_int32_t*)(hosts->h_addr_list[0]);
			return 0;
		}

		return -1;
	}
}