#ifndef TRANSPORTATION_COMMANDSEARCH_H
#define TRANSPORTATION_COMMANDSEARCH_H

#include "CMD.h"

namespace Transportation
{
	class CommandSearch: public Transportation::CMD
	{
		public:
		CommandSearch();
		bool operator()(const String::string &) const override;
	};
}

#endif //TRANSPORTATION_COMMANDSEARCH_H
