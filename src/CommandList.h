#ifndef TRANSPORTATION_COMMANDLIST_H
#define TRANSPORTATION_COMMANDLIST_H

#include "CMD.h"

namespace Transportation
{
	class CommandList: public Transportation::CMD
	{
		public:
		CommandList();
		bool operator()(const String::string &) const override;
	};
}

#endif //TRANSPORTATION_COMMANDLIST_H
