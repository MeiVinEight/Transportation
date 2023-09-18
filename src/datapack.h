#ifndef TRANSPORTATION_DATAPACK_H
#define TRANSPORTATION_DATAPACK_H

#include <WinType.h>
#include <stream.h>

namespace Transportation
{
	class ConnectionManager;
	namespace packet
	{
		class Datapack
		{
			public:
			const WORD ID;
			Datapack(WORD);
			virtual ~Datapack();
			virtual void operator()(Transportation::ConnectionManager &);
			virtual Transportation::packet::Datapack &operator<<(Streaming::stream &) = 0;
			virtual Transportation::packet::Datapack &operator>>(Streaming::stream &) = 0;
		};
	}
}

#endif //TRANSPORTATION_DATAPACK_H
