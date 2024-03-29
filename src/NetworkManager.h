#ifndef TRANSPORTATION_NETWORKMANAGER_H
#define TRANSPORTATION_NETWORKMANAGER_H

#include <ReentrantLock.h>

#include "ConnectionManager.h"

namespace Transportation
{
	class NetworkManager
	{
		public:
		WSA::Socket server;
		bool opening = false;
		QWORD length = 0;
		Concurrency::ReentrantLock lock;
		Transportation::ConnectionManager **connection = nullptr;
		virtual ~NetworkManager();
		void operator+=(Transportation::ConnectionManager *);
		void operator-=(Transportation::ConnectionManager *);
		Transportation::ConnectionManager *operator[](const String::string &);
		void operator~();
		void connect(const WSA::SocketAddress &);
		void close();
	};
}

#endif //TRANSPORTATION_NETWORKMANAGER_H
