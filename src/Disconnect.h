#ifndef TRANSPORTATION_DISCONNECT_H
#define TRANSPORTATION_DISCONNECT_H

#include <sstring.h>

#include "datapack.h"

namespace Transportation
{
	namespace packet
	{
		class Disconnect: public Transportation::packet::Datapack
		{
			public:
			String::string message;
			Disconnect();
			void operator()(Transportation::ConnectionManager &) const override;
			Transportation::packet::Disconnect &operator<<(Streaming::stream &) override;
			Transportation::packet::Disconnect &operator>>(Streaming::stream &) override;
		};
	} // Transportation
} // packet

#endif //TRANSPORTATION_DISCONNECT_H
