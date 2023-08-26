#ifndef TRANSPORTATION_TSTREAMING_H
#define TRANSPORTATION_TSTREAMING_H

#include <format.h>

namespace Transportation
{
	class streaming: public Streaming::format
	{
		public:
		streaming(Streaming::stream *);
		DWORD write(const void *, DWORD) override;
	};
} // Transportation

#endif //TRANSPORTATION_TSTREAMING_H
