#ifndef ADDRESS_H_
#define ADDRESS_H_

#include "platform.h"

namespace network
{
	class Address
	{
	public:
		Address();
		Address(const char* ip, u_int16_t port);

	public:
		static int String2Ip(const char * ip, u_int32_t & address);

	private:
		u_int32_t ip_;
		u_int16_t port_;

	};
}

#endif // !ADDRESS_H_
