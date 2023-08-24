#ifndef TRANSPORTATION_COMMANDMANAGER_H
#define TRANSPORTATION_COMMANDMANAGER_H

#include <WinType.h>

#include "CMD.h"

namespace Transportation
{
	class CommandManager
	{
		public:
		QWORD length = 0;
		const Transportation::CMD **CMD = new const Transportation::CMD *[0];
		CommandManager();
		~CommandManager();
		const Transportation::CMD *operator[](const String::string &) const;
		void operator+=(const Transportation::CMD *);
	};

} // Transportation

#endif //TRANSPORTATION_COMMANDMANAGER_H
