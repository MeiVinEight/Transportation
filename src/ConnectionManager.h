#ifndef TRANSPORTATION_CONNECTIONMANAGER_H
#define TRANSPORTATION_CONNECTIONMANAGER_H

#include <Socket.h>
#include <sstring.h>
#include <fully.h>

#include "datapack.h"

namespace Transportation
{
	class NetworkManager;
	class ConnectionManager
	{
		public:
		Transportation::NetworkManager *network;
		WSA::Socket connection;
		Streaming::fully stream;
		String::string name;
		ConnectionManager(Transportation::NetworkManager *, WSA::Socket);
		~ConnectionManager();
		Transportation::ConnectionManager &operator>>(Transportation::packet::Datapack *(&));
		Transportation::packet::Datapack *operator()();
		Transportation::ConnectionManager &operator<<(Transportation::packet::Datapack &);
		void operator()(Transportation::packet::Datapack &);
		void operator~();
	};
} // Transportation

#endif //TRANSPORTATION_CONNECTIONMANAGER_H