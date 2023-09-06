#ifndef TRANSPORTATION_HANDSHAKING_H
#define TRANSPORTATION_HANDSHAKING_H

#include <sstring.h>

#include "datapack.h"

namespace Transportation
{
	namespace packet
	{
		class Handshaking: public Transportation::packet::Datapack
		{
			public:
			static const DWORD ID = 0;
			WORD version;
			String::string name;
			Handshaking();
			void operator()(Transportation::ConnectionManager &) const override;
			Transportation::packet::Handshaking &operator<<(Streaming::stream &) override;
			Transportation::packet::Handshaking &operator>>(Streaming::stream &) override;
		};
	} // Transportation
} // packet

#endif //TRANSPORTATION_HANDSHAKING_H
