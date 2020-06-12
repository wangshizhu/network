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
		parser_.add("reuse_addr", '\0', "open reuse addr");
		parser_.add("no_accept", '\0', "no invoke accept()");
		parser_.add("skip_EOF", '\0', "no handle EOF message");
		parser_.add("ignore_RST",'\0', "Ignore RST err no,using it can test write data when recv a RST");
		parser_.add<int>("accept_sleep", '\0', "sleep time before accept(),unit:ms", false, 0);
		parser_.add<int>("handle_sleep", '\0', "sleep time when recv a message on proto of user side,unit:ms", false, 0);
		parser_.add<int>("send_byte_num", '\0', "send byte num from the memory per tick,0 is all,\n\t\t\t >0 is num", false, 0);
		parser_.add<int>("linger", '\0', "this arg is the second arg of linger,\n\t\t\t <0 dont set linger,>0 set linger, and the first arg is 1,\n\t\t\tthe second arg is that you give", false, -1);

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

	bool CommandLineParse::ReuseAddr()const
	{
		return parser_.exist("reuse_addr");
	}

	bool CommandLineParse::SkipEOF()const
	{
		return parser_.exist("skip_EOF");
	}

	int CommandLineParse::AcceptSleepTime()const
	{
		return parser_.get<int>("accept_sleep");
	}

	int CommandLineParse::HandleSleepTime() const 
	{
		return parser_.get<int>("handle_sleep");
	}

	int CommandLineParse::SendByteNum()const
	{
		return parser_.get<int>("send_byte_num");
	}

	bool CommandLineParse::IgnoreRST()const
	{
		return parser_.exist("ignore_RST");
	}

	int CommandLineParse::Linger()const
	{
		return parser_.get<int>("linger");
	}
}