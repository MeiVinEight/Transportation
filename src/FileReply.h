#ifndef TRANSPORTATION_FILEREPLY_H
#define TRANSPORTATION_FILEREPLY_H

#include <WinType.h>
#include <sstring.h>

#include "datapack.h"

namespace Transportation
{
	namespace packet
	{
		class FileReply: public Transportation::packet::Datapack
		{
			public:
			static const DWORD ID = 3;
			FileReply();
			String::string name;
			QWORD length = 0;
			unsigned char more = false;
			void operator()(Transportation::ConnectionManager &) override;
			Transportation::packet::FileReply &operator<<(Streaming::stream &) override;
			Transportation::packet::FileReply &operator>>(Streaming::stream &) override;
		};
	}
}

#endif //TRANSPORTATION_FILEREPLY_H
