#ifndef TRANSPORTATION_FILESEARCH_H
#define TRANSPORTATION_FILESEARCH_H

#include <sstring.h>

#include "datapack.h"

namespace Transportation
{
	namespace packet
	{
		class FileSearch: public Transportation::packet::Datapack
		{
			public:
			static const DWORD ID = 2;
			String::string name;
			FileSearch();
			void operator()(Transportation::ConnectionManager &) override;
			Transportation::packet::FileSearch &operator<<(Streaming::stream &) override;
			Transportation::packet::FileSearch &operator>>(Streaming::stream &) override;
		};
	}
}

#endif //TRANSPORTATION_FILESEARCH_H
