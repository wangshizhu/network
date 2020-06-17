#include "command_line.h"
#include "network_define.h"

namespace network
{
	CommandLineParse::CommandLineParse():parser_()
	{
	}

	void CommandLineParse::Parse(int argc, char **argv)
	{
		parser_.add<std::string>("host", 'h', "host name", false, "0.0.0.0");
		parser_.add<u_int16_t>("port", 'p', "port number", false, 5700, cmdline::range(1, 65535));
		parser_.add<int>("io_model", 'i', "io model,0:select,1:poll,2:epoll", false, 0);
		parser_.add("help", '?', "show usage");

		parser_.parse_check(argc, argv);
	}

	const char* CommandLineParse::Ip()
	{
		return parser_.get<std::string>("host").c_str();
	}

	const u_int16_t CommandLineParse::Port()
	{
		return parser_.get<u_int16_t>("port");
	}


	const int CommandLineParse::IOModel()
	{
		return parser_.get<int>("io_model");
	}
}