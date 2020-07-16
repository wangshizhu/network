#ifndef EVENT_POLLER_H_
#define EVENT_POLLER_H_

#include "network_center.h"
#include "interface.h"

#define POLL_INIT_SIZE 1000
#define EPOLL_EVENT_SIZE 10

namespace network
{
	class EventPoller
	{
	public:
		EventPoller();
		virtual ~EventPoller();

	public:
		virtual bool RegisterRead(int fd, SharedInputHandlerType handler) = 0;
		virtual bool DeregisterRead(int fd) = 0;
		virtual bool RegisterWrite(int fd, SharedOutputHandlerType handler) = 0;
		virtual bool DeregisterWrite(int fd) = 0;
		virtual int ProcessEvent() = 0;

	protected:
		int MaxFd()const;

		bool ProcessRead(int fd);
		bool ProcessWrite(int fd);
		bool ProcessError(int fd);

		bool IsRegistered(int fd, bool is_read)const;

	private:
		InputMapType in_map;
		OutputMapType out_map;
	};

	class SelectPoller : public EventPoller
	{
	public:
		SelectPoller();
		~SelectPoller();

	public:
		virtual bool RegisterRead(int fd, SharedInputHandlerType handler) override;
		virtual bool DeregisterRead(int fd) override;
		virtual bool RegisterWrite(int fd, SharedOutputHandlerType handler) override;
		virtual bool DeregisterWrite(int fd) override;
		virtual int ProcessEvent() override;

	private:
		void HandleReadyFd(int& ready_num, fd_set& read_fds, fd_set& write_fds);

	private:
		fd_set						fd_read_set_;
		fd_set						fd_write_set_;

		// ×îºó×¢²áµÄsocketÃèÊö·û £¨¶Á»òÐ´£©
		int							fd_largest_;

		// ×¢²áÐ´µÄsocketÃèÊö·ûÊýÁ¿
		int							fd_write_count_;
	};

#if GENERAL_PLATFORM == UNIX_FLAVOUR_LINUX

	class PollPoller : public EventPoller
	{
	public:
		PollPoller();
		~PollPoller();

	public:
		virtual bool RegisterRead(int fd, SharedInputHandlerType handler) override;
		virtual bool DeregisterRead(int fd);
		virtual bool RegisterWrite(int fd, SharedOutputHandlerType handler) override;
		virtual bool DeregisterWrite(int fd) override;
		virtual int ProcessEvent() override;

	private:
		bool HandleFdEvent(struct pollfd& fd_event);
		const int GetIndexInBinaryFind(int dest_fd);

	private:
		struct pollfd event_set_[POLL_INIT_SIZE];
	};

	class EpollPoller : public EventPoller
	{
	public:
		EpollPoller();
		~EpollPoller();

	public:
		virtual bool RegisterRead(int fd, SharedInputHandlerType handler) override;
		virtual bool DeregisterRead(int fd);
		virtual bool RegisterWrite(int fd, SharedOutputHandlerType handler) override;
		virtual bool DeregisterWrite(int fd) override;
		virtual int ProcessEvent() override;

	private:
		bool RegisterEvent(int fd, bool is_read, bool is_register);

	private:
		int epoll_fd_;
	};

#endif
}

#endif // !EVENT_POLLER_H_