#ifndef TRANSPORTATION_COMMANDSTOP_H
#define TRANSPORTATION_COMMANDSTOP_H

#include "CMD.h"

namespace Transportation
{
	class CommandStop: public Transportation::CMD
	{
		public:
		CommandStop();
		bool operator()(const String::string &) const override;
	};
}


#endif //TRANSPORTATION_COMMANDSTOP_H
