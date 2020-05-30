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
		parser_.add<std::string>("dest_host", '\0', "dest host name", false, "192.168.62.79");
		parser_.add<u_int16_t>("dest_port", '\0', "dest port number", false, 5700, cmdline::range(1, 65535));
		parser_.add<int>("listen_backlog", '\0', "listen backlog", false, 0);
		parser_.add("help", '?', "show usage");
		parser_.add("no_accept", '\0', "no invoke accept()");

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

	const char* CommandLineParse::DestIp()
	{
		return parser_.get<std::string>("dest_host").c_str();
	}

	const u_int16_t CommandLineParse::DestPort()
	{
		return parser_.get<u_int16_t>("dest_port");
	}

	int CommandLineParse::ListenBacklog() const
	{
		return parser_.get<int>("listen_backlog");
	}

	bool CommandLineParse::NoAccept()const
	{
		return parser_.exist("no_accept");
	}
}