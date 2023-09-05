#ifndef TRANSPORTATION_TSTREAMING_H
#define TRANSPORTATION_TSTREAMING_H

#include <format.h>

namespace Transportation
{
	class streaming: public Streaming::format
	{
		private:
		Memory::string memory;
		public:
		streaming(Streaming::stream *);
		DWORD write(const void *, DWORD) override;
	};
} // Transportation

#endif //TRANSPORTATION_TSTREAMING_H
