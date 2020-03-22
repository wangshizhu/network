#include "event_poller.h"

network::EventPoller::EventPoller()
{
	in_map.clear();
	out_map.clear();
}

network::EventPoller::~EventPoller()
{
}

bool network::EventPoller::RegisterRead(int fd, SharedInputHandlerType handler)
{
	in_map[fd] = WeakInputHandlerType(handler);
	return true;
}

bool network::EventPoller::DeregisterRead(int fd)
{
	in_map.erase(fd);
	return true;
}

bool network::EventPoller::RegisterWrite(int fd, SharedOutputHandlerType handler)
{
	out_map[fd] = WeakOutputHandlerType(handler);
	return true;
}

bool network::EventPoller::DeregisterWrite(int fd)
{
	out_map.erase(fd);
	return true;
}

int network::EventPoller::MaxFd()const
{
	int read_max_fd = -1;

	InputMapType::const_iterator it_input = in_map.begin();
	while (it_input != in_map.end())
	{
		if (it_input->first > read_max_fd)
		{
			read_max_fd = it_input->first;
		}

		++it_input;
	}

	int write_max_fd = -1;

	OutputMapType::const_iterator it_output = out_map.begin();
	while (it_output != out_map.end())
	{
		if (it_output->first > write_max_fd)
		{
			write_max_fd = it_output->first;
		}

		++it_output;
	}

	return GENERAL_MAX(read_max_fd, write_max_fd);
}

bool network::EventPoller::ProcessRead(int fd)
{
	InputMapType::iterator it = in_map.find(fd);
	if (it == in_map.end())
	{
		return false;
	}

	auto tmp = it->second.lock();
	if (tmp == nullptr)
	{
		return false;
	}

	tmp->HandleInput(fd);

	return true;
}

bool network::EventPoller::ProcessWrite(int fd)
{
	OutputMapType::iterator it = out_map.find(fd);
	if (it == out_map.end())
	{
		return false;
	}

	auto tmp = it->second.lock();
	if (tmp == nullptr)
	{
		return false;
	}

	tmp->HandleOutput(fd);

	return true;
}

bool network::EventPoller::ProcessError(int fd)
{
	if (!ProcessRead(fd))
	{
		return ProcessWrite(fd);
	}

	return true;
}

network::SelectPoller::SelectPoller():EventPoller(),fd_read_set_(),
fd_write_set_(),fd_largest_(-1),fd_write_count_(0)
{
	FD_ZERO(&fd_read_set_);
	FD_ZERO(&fd_write_set_);
}

network::SelectPoller::~SelectPoller()
{
}

bool network::SelectPoller::RegisterRead(int fd, SharedInputHandlerType handler)
{
#if GENERAL_PLATFORM != PLATFORM_WIN32
	if ((fd < 0) || (FD_SETSIZE <= fd))
	{
		return false;
	}
#else
	if (fd_read_set_.fd_count >= FD_SETSIZE)
	{
		return false;
	}
#endif

	if (FD_ISSET(fd, &fd_read_set_))
		return false;

	FD_SET(fd, &fd_read_set_);

	if (fd > fd_largest_)
	{
		fd_largest_ = fd;
	}

	EventPoller::RegisterRead(fd, handler);

	return true;
}

bool network::SelectPoller::DeregisterRead(int fd)
{
	EventPoller::DeregisterRead(fd);

#if GENERAL_PLATFORM != PLATFORM_WIN32
	if ((fd < 0) || (FD_SETSIZE <= fd))
	{
		return false;
	}
#endif

	if (!FD_ISSET(fd, &fd_read_set_))
	{
		return false;
}

	FD_CLR(fd, &fd_read_set_);

	if (fd == fd_largest_)
	{
		fd_largest_ = MaxFd();
	}

	return true;
}

bool network::SelectPoller::RegisterWrite(int fd, SharedOutputHandlerType handler)
{
#if GENERAL_PLATFORM != PLATFORM_WIN32
	if ((fd < 0) || (FD_SETSIZE <= fd))
	{
		return false;
	}
#else
	if (fd_write_set_.fd_count >= FD_SETSIZE)
	{
		return false;
	}
#endif

	if (FD_ISSET(fd, &fd_write_set_))
	{
		return false;
	}

	FD_SET(fd, &fd_write_set_);

	if (fd > fd_largest_)
	{
		fd_largest_ = fd;
	}

	++fd_write_count_;

	EventPoller::RegisterWrite(fd, handler);

	return true;
}

bool network::SelectPoller::DeregisterWrite(int fd)
{
	EventPoller::DeregisterWrite(fd);

#if GENERAL_PLATFORM != PLATFORM_WIN32
	if ((fd < 0) || (FD_SETSIZE <= fd))
	{
		return false;
	}
#endif

	if (!FD_ISSET(fd, &fd_write_set_))
	{
		return false;
	}

	FD_CLR(fd, &fd_write_set_);

	if (fd == fd_largest_)
	{
		fd_largest_ = MaxFd();
	}

	--fd_write_count_;

	return true;
}

int network::SelectPoller::ProcessEvent()
{

	fd_set	read_fds;
	fd_set	write_fds;

	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);

	read_fds = fd_read_set_;
	write_fds = fd_write_set_;

	int num = 0;

#if GENERAL_PLATFORM == PLATFORM_WIN32
	if (fd_largest_ == -1)
	{
		GeneralSleep(500);
	}
	else
#endif
	{
		num = select(fd_largest_ + 1, &read_fds,fd_write_count_ ? &write_fds : nullptr, nullptr,nullptr);
	}

	if (num > 0)
	{
		this->HandleReadyFd(num, read_fds, write_fds);
	}
	else if (num == -1)
	{
	}

	return num;
}

void network::SelectPoller::HandleReadyFd(int& ready_num, fd_set& read_fds, fd_set& write_fds)
{
#if GENERAL_PLATFORM == PLATFORM_WIN32
	for (unsigned i = 0; i < read_fds.fd_count; ++i)
	{
		int fd = read_fds.fd_array[i];
		--ready_num;
		this->ProcessRead(fd);
	}

	for (unsigned i = 0; i < write_fds.fd_count; ++i)
	{
		int fd = write_fds.fd_array[i];
		--ready_num;
		this->ProcessWrite(fd);
	}

#else
	for (int fd = 0; fd <= fd_largest_ && ready_num > 0; ++fd)
	{
		if (FD_ISSET(fd, &fd_read_set_))
		{
			--ready_num;
			this->ProcessRead(fd);
		}

		if (FD_ISSET(fd, &write_fds))
		{
			--ready_num;
			this->ProcessWrite(fd);
		}
	}
#endif
}