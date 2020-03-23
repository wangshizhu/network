#include "event_processor.h"

namespace network
{
	EventProcessor::EventProcessor(int poller)
	{
#if GENERAL_PLATFORM == PLATFORM_WIN32
		poller = (int)EnumPoller::SELECT_POLLER;
#endif
		if (poller == (int)EnumPoller::SELECT_POLLER)
		{
			poller_ = std::make_unique<SelectPoller>();
		}
		else if (poller == (int)EnumPoller::POLL_POLLER)
		{
		}
		else if (poller == (int)EnumPoller::EPOLL_POLLER)
		{
		}
		else
		{

		}
	}

	EventProcessor::~EventProcessor()
	{
	}

	void EventProcessor::ProcessEvent()
	{
		poller_->ProcessEvent();
	}

	bool EventProcessor::RegisterRead(int fd, SharedInputHandlerType handler)
	{
		return poller_->RegisterRead(fd, handler);
	}

	bool EventProcessor::DeregisterRead(int fd)
	{
		return poller_->DeregisterRead(fd);
	}

	bool EventProcessor::RegisterWrite(int fd, SharedOutputHandlerType handler)
	{
		return poller_->RegisterWrite(fd, handler);
	}

	bool EventProcessor::DeregisterWrite(int fd)
	{
		return poller_->DeregisterWrite(fd);
	}
}