#ifndef TRANSPORTATION_CONNECTIONMANAGER_H
#define TRANSPORTATION_CONNECTIONMANAGER_H

#include <Socket.h>
#include <sstring.h>
#include <fully.h>

#include "datapack.h"

namespace Transportation
{
	class ConnectionManager
	{
		public:
		WSA::Socket connection;
		Streaming::fully stream;
		String::string name;
		ConnectionManager(WSA::Socket);
		Transportation::ConnectionManager &operator>>(Transportation::packet::Datapack *(&));
		Transportation::packet::Datapack *operator()();
		Transportation::ConnectionManager &operator<<(const Transportation::packet::Datapack &);
		void operator()(const Transportation::packet::Datapack &);
	};
} // Transportation

#endif //TRANSPORTATION_CONNECTIONMANAGER_H