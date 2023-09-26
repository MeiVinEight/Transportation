#include <timestamp.h>
#include <cthread.h>

#include "Tstreaming.h"
#include "atomic.h"

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
		this->lock();
		bool LF = false;
		const BYTE *cur = beg;
		while (cur < end && !(LF |= *cur++ == '\n'));
		QWORD pos = this->memory.length;
		this->memory.resize(pos + (cur - beg));
		Memory::copy(this->memory.address + pos, beg, cur - beg);
		if (LF)
		{
			this->Streaming::format::write("\u001B7\n\u001B8\x1B[1B\u001B7\x1B[1A\x1B[1G\x1B[1L", 23);
			this->Streaming::format::write(Transportation::streaming::time().address, 11);
			ret += this->Streaming::format::write(this->memory.address, this->memory.length);
			this->Streaming::format::write("\u001B8", 2);
			this->memory.resize(0);

			this->unlock();
		}
		beg = cur;
	}

	return ret;
}
void Transportation::streaming::lock()
{
	// check whether current occupied the lock
	DWORD current = WTM::thread::current().ID();
	if (this->exclusive != current)
	{
		// if not, try to acquire the lock, use thread id as locker
		while (_InterlockedCompareExchange((long volatile *) &this->exclusive, (long) current, 0))
		{
			// self spin
			WTM::thread::sleep(1);
		}
	}
}
void Transportation::streaming::unlock()
{
	// clear occupier
	this->exclusive = 0;
}
Memory::string Transportation::streaming::time()
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
	Memory::string ret(11);
	Memory::copy(ret.address, prefix, 11);
	return ret;
}