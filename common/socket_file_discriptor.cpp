#include "socket_file_discriptor.h"
#include "../lib/format.h"

static bool g_network_initted = false;

network::SocketWrapper::SocketWrapper() :
#if GENERAL_PLATFORM == PLATFORM_WIN32
socket_(INVALID_SOCKET)
#else
socket_(-1)
#endif
{
}

network::SocketWrapper::SocketWrapper(const char* ip, u_int16_t port):
#if GENERAL_PLATFORM == PLATFORM_WIN32
	socket_(INVALID_SOCKET),
#else
	socket_(-1),
#endif
	addr_(ip,port)
{
}

network::SocketWrapper::~SocketWrapper()
{
	DEBUG_INFO("SocketWrapper have released fd:{0}\n", socket_);
	Close();
}

void network::SocketWrapper::CreateSocket(int type)
{
	InittdNetWork();
	socket_ = (int)::socket(AF_INET, type, 0);
}

bool network::SocketWrapper::IsGood()const
{
#if GENERAL_PLATFORM == PLATFORM_WIN32
	return socket_ != INVALID_SOCKET;
#else
	return socket_ != -1;
#endif
}

void network::SocketWrapper::InittdNetWork()
{
	if (g_network_initted)
		return;

	g_network_initted = true;

#if GENERAL_PLATFORM == PLATFORM_WIN32
	WSAData wsdata;
	WSAStartup(0x202, &wsdata);
#endif
}

int network::SocketWrapper::Bind()
{
	// �󶨵�port��ip
	struct sockaddr_in server_sock_addr;

	// IPV4
	server_sock_addr.sin_family = AF_INET;
	// ָ���˿�
	server_sock_addr.sin_port = addr_.Port();

	server_sock_addr.sin_addr.s_addr = addr_.Ip();

	return ::bind(socket_, (struct sockaddr *) &server_sock_addr, sizeof(server_sock_addr));
}

int network::SocketWrapper::Listen()
{
	return ::listen(socket_, LISTENQ);
}

std::shared_ptr<network::SocketWrapper> network::SocketWrapper::Accept()
{
	struct sockaddr_in client_sock_addr;
	memset(&client_sock_addr, 0, sizeof(client_sock_addr));
	socklen_t cli_addr_len = sizeof(client_sock_addr);

	int ret = (int)::accept(socket_, (sockaddr*)&client_sock_addr, &cli_addr_len);

#if GENERAL_PLATFORM == PLATFORM_WIN32
	if (ret == INVALID_SOCKET)
	{
		return nullptr;
	}
#else
	if (ret < 0) 
	{
		return nullptr;
	}
#endif

	std::shared_ptr<SocketWrapper> new_sock = std::make_shared<SocketWrapper>();

	new_sock->SetSocket(ret);
	new_sock->SetNonBlocking(true);
	new_sock->SetNoDelay(true);

	return new_sock;
}

int network::SocketWrapper::SetNonBlocking(bool nonblocking)
{
#if GENERAL_PLATFORM == PLATFORM_WIN32
	u_long val = nonblocking ? 1 : 0;
	return ::ioctlsocket(socket_, FIONBIO, &val);
#else
	int val = nonblocking ? O_NONBLOCK : 0;
	return ::fcntl(socket_, F_SETFL, val);
#endif
}

int network::SocketWrapper::SetNoDelay(bool nodelay)
{
	int arg = int(nodelay);
	return setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, (char*)&arg, sizeof(int));
}

int network::SocketWrapper::SetLinger(uint16 onoff, uint16 linger)
{
	struct linger l = { 0 };
	l.l_onoff = onoff;
	l.l_linger = linger;
	return setsockopt(socket_, SOL_SOCKET, SO_LINGER, (const char *)&l, sizeof(l));
}

int network::SocketWrapper::GetSocket()
{
	return socket_;
}

network::Address network::SocketWrapper::GetRemoteAddress() const
{
	network::Address addr;

	sockaddr_in sin;
	socklen_t sinLen = sizeof(sin);

	int ret = ::getpeername(socket_, (struct sockaddr*)&sin, &sinLen);
	if (ret == 0)
	{
		addr.Ip(sin.sin_addr.s_addr);
		addr.Port(sin.sin_port);
	}
	else
	{
		ERROR_INFO("get remote address failed!!!");
	}

	return addr;
}

int network::SocketWrapper::Close()
{
	if (!IsGood())
	{
		return 0;
	}

	int ret = CLOSE_SOCKET(socket_);
	if (ret != 0)
	{
		return ret;
	}

#if GENERAL_PLATFORM == PLATFORM_WIN32
	const GENERALSOCKET invalid_socket = INVALID_SOCKET;
#else
	const GENERALSOCKET invalid_socket = -1;
#endif

	SetSocket(invalid_socket);

	return ret;
}

int network::SocketWrapper::Recv(void* gram_data, int gram_size)
{
	return ::recv(socket_, (char*)gram_data, gram_size, 0);
}

void network::SocketWrapper::SetSocket(GENERALSOCKET sock)
{
	socket_ = sock;
}

int network::SocketWrapper::Send(const void * gram_data, int gram_size)
{
	return ::send(socket_, (char*)gram_data, gram_size,0);
}

int network::SocketWrapper::Connect()
{
	struct sockaddr_in server_sock_addr;
	memset(&server_sock_addr, 0, sizeof(server_sock_addr));

	server_sock_addr.sin_family = AF_INET;
	server_sock_addr.sin_port = addr_.Port();
	server_sock_addr.sin_addr.s_addr = addr_.Ip();

	return ::connect(socket_, (struct sockaddr*)&server_sock_addr, sizeof(server_sock_addr));
}