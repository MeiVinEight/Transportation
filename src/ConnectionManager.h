#ifndef TRANSPORTATION_CONNECTIONMANAGER_H
#define TRANSPORTATION_CONNECTIONMANAGER_H

#include <Socket.h>
#include <sstring.h>
#include <fully.h>
#include <ReentrantLock.h>

#include "datapack.h"

namespace Transportation
{
	class NetworkManager;
	class ConnectionManager
	{
		public:
		static const char CLOSED      = 0;
		static const char HANDSHAKING = 1;
		static const char PLAYING     = 2;
		Transportation::NetworkManager *network;
		WSA::Socket connection;
		Streaming::fully stream;
		const WSA::SocketAddress address;
		String::string name;
		Concurrency::ReentrantLock IL;
		Concurrency::ReentrantLock OL;
		char opening = CLOSED;
		long waiting = 0;
		ConnectionManager(Transportation::NetworkManager *, WSA::Socket);
		~ConnectionManager();
		void operator++(int);
		void operator--(int);
		Transportation::ConnectionManager &operator>>(Transportation::packet::Datapack *(&));
		Transportation::packet::Datapack *operator()();
		Transportation::ConnectionManager &operator<<(Transportation::packet::Datapack &);
		void operator()(Transportation::packet::Datapack &);
		void operator~();
		void close(const String::string &);
	};
} // Transportation

#endif //TRANSPORTATION_CONNECTIONMANAGER_H