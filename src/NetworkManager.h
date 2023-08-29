#ifndef TRANSPORTATION_NETWORKMANAGER_H
#define TRANSPORTATION_NETWORKMANAGER_H

#include "ConnectionManager.h"

namespace Transportation
{
	class NetworkManager
	{
		public:
		QWORD length = 0;
		Transportation::ConnectionManager **connection = nullptr;
		virtual ~NetworkManager();
		void operator+=(Transportation::ConnectionManager *);
		Transportation::ConnectionManager *operator[](const String::string &) const;
	};
}

#endif //TRANSPORTATION_NETWORKMANAGER_H
