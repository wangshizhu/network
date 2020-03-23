#ifndef EVENT_PROCESSOR_H_
#define EVENT_PROCESSOR_H_

#include "singleton.h"
#include "event_poller.h"
#include "interface.h"

namespace network
{
	class EventPoller;
	class InputHandler;
	class OutputHandler;
	using SharedInputHandlerType = std::shared_ptr<InputHandler>;
	using SharedOutputHandlerType = std::shared_ptr<OutputHandler>;

	class EventProcessor : public Singleton<EventProcessor>
	{
	public:
		EventProcessor(int poller);
		~EventProcessor();

	public:
		void ProcessEvent();
		bool RegisterRead(int fd, SharedInputHandlerType handler);
		bool DeregisterRead(int fd);
		bool RegisterWrite(int fd, SharedOutputHandlerType handler);
		bool DeregisterWrite(int fd);

	private:
		std::unique_ptr<EventPoller> poller_;
	};
}

#endif // !EVENT_PROCESSOR_H_