#ifndef TRANSPORTATION_CMD_H
#define TRANSPORTATION_CMD_H

#include <sstring.h>

namespace Transportation
{
	class CMD
	{
		public:
		const String::string ID;
		CMD(String::string );
		virtual ~CMD();
		virtual bool operator()(const String::string &) const = 0;
	};
} // Transportation

#endif //TRANSPORTATION_CMD_H
