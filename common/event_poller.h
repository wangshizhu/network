#ifndef EVENT_POLLER_H_
#define EVENT_POLLER_H_

#include "network_center.h"
#include "interface.h"

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
		virtual bool RegisterRead(int fd, SharedInputHandlerType handler);
		virtual bool DeregisterRead(int fd);
		virtual bool RegisterWrite(int fd, SharedOutputHandlerType handler);
		virtual bool DeregisterWrite(int fd);
		virtual int ProcessEvent();

	private:
		void HandleReadyFd(int& ready_num, fd_set& read_fds, fd_set& write_fds);

	private:
		fd_set						fd_read_set_;
		fd_set						fd_write_set_;

		// ×îºó×¢²áµÄsocketÃèÊö·û £¨¶Á»òĞ´£©
		int							fd_largest_;

		// ×¢²áĞ´µÄsocketÃèÊö·ûÊıÁ¿
		int							fd_write_count_;
	};
}

#endif // !EVENT_POLLER_H_