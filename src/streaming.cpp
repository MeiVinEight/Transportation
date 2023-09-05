#include <timestamp.h>

#include "Tstreaming.h"

Transportation::streaming::streaming(Streaming::stream *stream): Streaming::format(stream)
{
}
DWORD Transportation::streaming::write(const void *b, DWORD length)
{
	const BYTE *beg = (const BYTE *) b;
	const BYTE *end = beg + length;
	QWORD ret = 0;
	while (beg < end)
	{
		bool LF = false;
		const BYTE *cur = beg;
		while (cur < end && !(LF |= *cur++ == '\n'));
		QWORD pos = this->memory.length;
		this->memory.resize(pos + (cur - beg));
		Memory::copy(this->memory.address + pos, beg, cur - beg);
		if (LF)
		{
			char prefix[] = "[00:00:00] ";
			QWORD time = Timestamp::current();
			time /= 1000;
			QWORD SS = time % 60;
			time /= 60;
			QWORD MM = time % 60;
			time /= 60;
			QWORD HH = time % 24;
			prefix[1] = (char) ('0' + (HH / 10));
			prefix[2] = (char) ('0' + (HH % 10));
			prefix[4] = (char) ('0' + (MM / 10));
			prefix[5] = (char) ('0' + (MM % 10));
			prefix[7] = (char) ('0' + (SS / 10));
			prefix[8] = (char) ('0' + (SS % 10));
			this->Streaming::format::write("\x1B[1S\u001B7\x1B[1A\x1B[1G\x1B[1L", 18);
			this->Streaming::format::write(prefix, 11);
			ret += this->Streaming::format::write(this->memory.address, this->memory.length);
			this->Streaming::format::write("\u001B8", 2);
			this->memory.resize(0);
		}
		beg = cur;
	}

	return ret;
}