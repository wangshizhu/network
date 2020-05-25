#include "command_line.h"
#include "network_define.h"

namespace network
{
	CommandLineParse::CommandLineParse()
	{
		ip_ = new char[MAX_IP_LEN];
		port_ = 0;
	}

	CommandLineParse::~CommandLineParse()
	{
		SAFE_RELEASE_ARRAY(ip_);
	}

	void CommandLineParse::Parse(int argc, char **argv)
	{
		for (int i=0;i<argc;i++)
		{
			if (strcmp(argv[i],"-h") == 0)
			{
				if ((i + 1) < argc)
				{
				}
			}

			if (!strcmp(argv[i],"-p") == 0 && (i + 1) < argc)
			{

			}
		}
	}

	const char* CommandLineParse::Ip()
	{
		return ip_;
	}

	const u_int16_t CommandLineParse::Port()
	{
		return port_;
	}
}