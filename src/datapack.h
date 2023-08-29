#ifndef TRANSPORTATION_DATAPACK_H
#define TRANSPORTATION_DATAPACK_H

#include <WinType.h>

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
			virtual void operator()(Transportation::ConnectionManager &) const;
		};
	}
}

#endif //TRANSPORTATION_DATAPACK_H
