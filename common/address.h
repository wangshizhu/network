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
		u_int32_t Ip()const;
		u_int16_t Port()const;
		void Ip(u_int32_t ip);
		void Port(u_int16_t port);

		static int String2Ip(const char * ip, u_int32_t & address);
		static int Ip2String(u_int32_t address, char * string);

	private:
		u_int32_t ip_;
		u_int16_t port_;

	};
}

#endif // !ADDRESS_H_
