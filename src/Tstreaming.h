#ifndef TRANSPORTATION_TSTREAMING_H
#define TRANSPORTATION_TSTREAMING_H

#include <format.h>

namespace Transportation
{
	class streaming: public Streaming::format
	{
		private:
		Memory::string memory;
		volatile DWORD exclusive = 0;
		public:
		streaming(Streaming::stream *);
		DWORD write(const void *, DWORD) override;
		void lock();
		void unlock();
		static Memory::string time();
	};
} // Transportation

#endif //TRANSPORTATION_TSTREAMING_H
