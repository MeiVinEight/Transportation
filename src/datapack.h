#ifndef TRANSPORTATION_DATAPACK_H
#define TRANSPORTATION_DATAPACK_H

#include <WinType.h>

namespace Transportation
{
	namespace packet
	{
		class Datapack
		{
			public:
			const WORD ID;
			Datapack(WORD);
			virtual ~Datapack();
		};
	}
}

#endif //TRANSPORTATION_DATAPACK_H
