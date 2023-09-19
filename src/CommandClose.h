#ifndef TRANSPORTATION_COMMANDCLOSE_H
#define TRANSPORTATION_COMMANDCLOSE_H

#include "CMD.h"

namespace Transportation
{
	class CommandClose: public Transportation::CMD
	{
		public:
		CommandClose();
		bool operator()(const String::string &) const override;
	};
}

#endif //TRANSPORTATION_COMMANDCLOSE_H
