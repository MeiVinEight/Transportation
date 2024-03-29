#ifndef TRANSPORTATION_PROTOCOL_H
#define TRANSPORTATION_PROTOCOL_H

#include <WinType.h>

#include "datapack.h"

namespace Transportation
{
	namespace protocol
	{
		extern Transportation::packet::Datapack *(*(datapack[]))();
		extern WORD version;
	}
}
#endif //TRANSPORTATION_PROTOCOL_H
