#ifndef TRANSPORTATION_COMMANDCONNECT_H
#define TRANSPORTATION_COMMANDCONNECT_H

#include "CMD.h"

namespace Transportation
{
	class CommandConnect: public Transportation::CMD
	{
		public:
		__declspec(dllexport) CommandConnect();
		__declspec(dllexport) bool operator()(const String::string &) const override;
	};
}

#endif //TRANSPORTATION_COMMANDCONNECT_H
