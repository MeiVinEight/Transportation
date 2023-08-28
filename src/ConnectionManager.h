#ifndef TRANSPORTATION_CONNECTIONMANAGER_H
#define TRANSPORTATION_CONNECTIONMANAGER_H

#include <Socket.h>
#include <sstring.h>

namespace Transportation
{
	class ConnectionManager
	{
		public:
		const WSA::Socket connection;
		String::string name;
		ConnectionManager(WSA::Socket);
	};
} // Transportation

#endif //TRANSPORTATION_CONNECTIONMANAGER_H