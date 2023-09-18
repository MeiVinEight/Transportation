#ifndef TRANSPORTATION_COMMANDCONNECT_H
#define TRANSPORTATION_COMMANDCONNECT_H

#include "CMD.h"

namespace Transportation
{
	class CommandConnect: public Transportation::CMD
	{
		public:
		CommandConnect();
		bool operator()(const String::string &) const override;
	};
}

#endif //TRANSPORTATION_COMMANDCONNECT_H
