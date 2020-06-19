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
	in_map[fd] = handler;
	return true;
}

bool network::EventPoller::DeregisterRead(int fd)
{
	if (!IsRegistered(fd, false))
	{
		return false;
	}

	in_map.erase(fd);

	return true;
}

bool network::EventPoller::RegisterWrite(int fd, SharedOutputHandlerType handler)
{
	out_map[fd] = handler;
	return true;
}

bool network::EventPoller::DeregisterWrite(int fd)
{
	if (!IsRegistered(fd,false))
	{
		return false;
	}
	
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

bool network::EventPoller::IsRegistered(int fd, bool is_read)const
{
	return is_read ? (in_map.find(fd) != in_map.end()) : (out_map.find(fd) != out_map.end());
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
	if (!EventPoller::DeregisterRead(fd))
	{
		return false;
	}

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
	if (IsRegistered(fd,false))
	{
		return false;
	}

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
	if (!EventPoller::DeregisterWrite(fd))
	{
		return false;
	}

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

	struct timeval tv;
	// TODO: select 超时时间有待修改
	tv.tv_sec = 1;
	tv.tv_usec = 500;

	int num = 0;

#if GENERAL_PLATFORM == PLATFORM_WIN32
	if (fd_largest_ == -1)
	{
		GeneralSleep(500);
	}
	else
#endif
	{
		num = select(fd_largest_ + 1, &read_fds,fd_write_count_ ? &write_fds : nullptr, nullptr, &tv);
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
		if (FD_ISSET(fd, &read_fds))
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

#if GENERAL_PLATFORM == UNIX_FLAVOUR_LINUX
network::PollPoller::PollPoller()
{
	for (int i = 0;i<POLL_INIT_SIZE;i++)
	{
		event_set_[i].fd = -1;
		event_set_[i].events = 0;
		event_set_[i].revents = 0;
	}
}

network::PollPoller::~PollPoller()
{

}

bool network::PollPoller::RegisterRead(int fd, SharedInputHandlerType handler)
{
	int index = GetIndexInBinaryFind(fd);
	if (index == -1)
	{
		int empty_fd = -1;
		index = GetIndexInBinaryFind(empty_fd);
	}

	if (index == -1)
	{
		return false;
	}

	event_set_[index].fd = fd;
	event_set_[index].events |= POLLIN;

	EventPoller::RegisterRead(fd, handler);

	return true;
}

bool network::PollPoller::DeregisterRead(int fd)
{
	if (!EventPoller::DeregisterRead(fd))
	{
		return false;
	}

	int index = GetIndexInBinaryFind(fd);
	if (index == -1)
	{
		return false;
	}

	event_set_[index].fd = fd;
	event_set_[index].events ^= POLLIN;

	return true;
}

bool network::PollPoller::RegisterWrite(int fd, SharedOutputHandlerType handler)
{
	if (IsRegistered(fd, false))
	{
		return false;
	}

	int index = GetIndexInBinaryFind(fd);
	if (index == -1)
	{
		int empty_fd = -1;
		index = GetIndexInBinaryFind(empty_fd);
	}

	if (index == -1)
	{
		return false;
	}

	event_set_[index].fd = fd;
	event_set_[index].events |= POLLOUT;

	EventPoller::RegisterWrite(fd, handler);

	return true;
}

bool network::PollPoller::DeregisterWrite(int fd)
{
	if (!EventPoller::DeregisterWrite(fd))
	{
		return false;
	}

	int index = GetIndexInBinaryFind(fd);
	if (index == -1)
	{
		return false;
	}

	event_set_[index].fd = fd;
	event_set_[index].events ^= POLLOUT;

	return true;
}

int network::PollPoller::ProcessEvent()
{
	int poll_timeout = 100;
	
	int num = ::poll(event_set_,POLL_INIT_SIZE, poll_timeout);

	if (num > 0)
	{
		std::size_t i = 0;
		std::size_t j = POLL_INIT_SIZE - 1;

		while (true)
		{
			if (j < i)
			{
				break;
			}

			if (HandleFdEvent(event_set_[i]))
			{
				if (--num <= 0)
				{
					break;
				}
			}

			if (i == j)
			{
				break;
			}

			if (HandleFdEvent(event_set_[j]))
			{
				if (--num <= 0)
				{
					break;
				}
			}

			++i;
			--j;
		}
	}
	else if (num == -1)
	{
		int err_no = CatchLastError();
		ERROR_INFO("poll return value:{0}", err_no);
	}

	return 0;
}

bool network::PollPoller::HandleFdEvent(struct pollfd& fd_event)
{
	bool have = false;
	if (fd_event.revents  & (POLLERR | POLLHUP))
	{
		have = true;

		this->ProcessError(fd_event.fd);

		return have;
	}

	if (fd_event.revents & (POLLIN))
	{
		this->ProcessRead(fd_event.fd);
		have = true;
	}
	if (fd_event.revents & (POLLOUT))
	{
		this->ProcessWrite(fd_event.fd);
		have = true;
	}

	return have;
}

const int network::PollPoller::GetIndexInBinaryFind(int dest_fd)
{
	std::size_t i = 0;
	std::size_t j = POLL_INIT_SIZE - 1;

	while (true)
	{
		if (j < i)
		{
			return -1;
		}
		if (event_set_[i].fd == dest_fd)
		{
			return i;
		}
		if (i == j)
		{
			return -1;
		}
		if (event_set_[j].fd == dest_fd)
		{
			return j;
		}

		++i;
		--j;
	}

	return -1;
}

network::EpollPoller::EpollPoller():epoll_fd_(epoll_create(1))
{
	if (epoll_fd_ == -1)
	{
		ERROR_INFO("epoll create failed,err no:{0}", CatchLastError());
	}
}

network::EpollPoller::~EpollPoller()
{
	if (epoll_fd_ != -1)
	{
		close(epoll_fd_);
	}
}

bool network::EpollPoller::RegisterRead(int fd, SharedInputHandlerType handler)
{
	if (!RegisterEvent(fd,true,true))
	{
		return false;
	}

	EventPoller::RegisterRead(fd, handler);

	return true;
}

bool network::EpollPoller::DeregisterRead(int fd)
{
	if (!EventPoller::DeregisterRead(fd))
	{
		return false;
	}

	if (!RegisterEvent(fd, true, false))
	{
		return false;
	}

	return true;
}

bool network::EpollPoller::RegisterWrite(int fd, SharedOutputHandlerType handler)
{
	if (IsRegistered(fd, false))
	{
		return false;
	}

	if (!RegisterEvent(fd, false, true))
	{
		return false;
	}

	EventPoller::RegisterWrite(fd, handler);

	return true;
}

bool network::EpollPoller::DeregisterWrite(int fd)
{
	if (!EventPoller::DeregisterWrite(fd))
	{
		return false;
	}

	if (!RegisterEvent(fd, false, false))
	{
		return false;
	}

	return true;
}

int network::EpollPoller::ProcessEvent()
{
	struct epoll_event events[EPOLL_EVENT_SIZE];

	int num = epoll_wait(epoll_fd_, events, EPOLL_EVENT_SIZE, 100);

	for (int i = 0; i < num; ++i)
	{
		if (events[i].events & (EPOLLERR | EPOLLHUP))
		{
			this->ProcessError(events[i].data.fd);
		}
		else
		{
			if (events[i].events & EPOLLIN)
			{
				this->ProcessRead(events[i].data.fd);
			}

			if (events[i].events & EPOLLOUT)
			{
				this->ProcessWrite(events[i].data.fd);
			}
		}
	}

	return num;
}

bool network::EpollPoller::RegisterEvent(int fd, bool is_read, bool is_register)
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.data.fd = fd;

	int op;

	if (this->IsRegistered(fd, !is_read))
	{
		op = EPOLL_CTL_MOD;

		ev.events = is_register ? EPOLLIN | EPOLLOUT : is_read ? EPOLLOUT : EPOLLIN;
	}
	else
	{
		op = is_register ? EPOLL_CTL_ADD : EPOLL_CTL_DEL;

		ev.events = is_read ? EPOLLIN : EPOLLOUT;
	}

	if (epoll_ctl(epoll_fd_, op, fd, &ev) < 0)
	{
		ERROR_INFO("register event failed,fd:{0},read or write:{1},register or deregister:{2}",fd,(int)is_read,(int)is_register);

		return false;
	}

	return true;
}

#endif

