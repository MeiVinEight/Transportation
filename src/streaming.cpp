#include <standardstream.h>
#include <timestamp.h>

#include "Tstreaming.h"

Transportation::streaming::streaming(Streaming::stream *stream): Streaming::format(stream)
{
}
DWORD Transportation::streaming::write(const void *b, DWORD length)
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

	char LF = '\n';

	QWORD ret = 0;
	ret += this->Streaming::format::write("\x1B[1G\x1B[2K", 8);
	ret += this->Streaming::format::write(prefix, 11);
	ret += this->Streaming::format::write(b, length);
	ret += this->Streaming::format::write(&LF, 1);
	ret += this->Streaming::format::write("> ", 2);
	return ret;
}