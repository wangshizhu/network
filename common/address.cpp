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

	u_int32_t Address::Ip() const 
	{
		return ip_;
	}

	u_int16_t Address::Port() const
	{
		return port_;
	}

	void Address::Ip(u_int32_t ip)
	{
		ip_ = ip;
	}

	void Address::Port(u_int16_t port)
	{
		port_ = port;
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

	int Address::Ip2String(u_int32_t address, char * out_str)
	{
		address = ntohl(address);

		int p1, p2, p3, p4;
		p1 = address >> 24;
		p2 = (address & 0xffffff) >> 16;
		p3 = (address & 0xffff) >> 8;
		p4 = (address & 0xff);

		return sprintf(out_str, "%d.%d.%d.%d", p1, p2, p3, p4);
	}
}