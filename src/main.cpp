#include <sstring.h>
#include <streaming.h>
#include <standardstream.h>
#include <cthread.h>

#include "Transportation.h"
#include "CMD.h"
#include "CommandStop.h"

DWORD T01C(void *)
{
	bool running = true;
	while (running)
	{
		String::string cmd;
		Streaming::cin >> cmd;

		QWORD buflen = 64;
		char *buf = new char[buflen];
		QWORD idx = 0;

		bool continua = true;
		bool whitespace = false;
		while (continua)
		{
			char ch;
			Streaming::cin >> ch;
			continua &= ch != '\r' && ch != '\n';
			whitespace |= ch > 0x20;
			if (continua && whitespace)
			{
				if (idx >= buflen)
				{
					char *nBuf = new char[buflen << 1];
					Memory::copy(nBuf, buf, buflen);
					delete[] buf;
					buf = nBuf;
					buflen <<= 1;
				}
				buf[idx++] = ch;
			}
		}
		String::string options(buf, buflen);
		delete[] buf;
		const Transportation::CMD *obj = Transportation::command[cmd];
		running = (!obj) || (*obj)(options);
	}
	return 0;
}

int main()
{
	Transportation::command += new Transportation::CommandStop();
	WTM::thread::create(T01C, nullptr);
	return 0;
}