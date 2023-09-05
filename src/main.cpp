#include <sstring.h>
#include <standardstream.h>
#include <cthread.h>
#include <streaming.h>
#include <stringstream.h>

#include "Transportation.h"
#include "CMD.h"
#include "CommandStop.h"

DWORD T01C(void *)
{
	bool running = true;
	while (running)
	{
		Transportation::cout.Streaming::format::write("> ", 2);

		QWORD buflen = 64;
		char *buf = new char[buflen];
		QWORD idx = 0;

		bool continua = true;
		bool whitespace = false;
		while (continua)
		{
			char ch;
			Streaming::cin >> ch;
			continua &= ch != '\r'/* && ch != '\n'*/;
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
		Streaming::string str;
		str.address.resize(idx);
		Memory::copy(str.address.address, buf, idx);
		delete[] buf;

		Streaming::format strin(&str);

		String::string cmd;
		strin >> cmd;
		String::string options(str.address.address + str.position, str.available());

		const Transportation::CMD *obj = Transportation::command[cmd];
		running = (!obj) || (*obj)(options);
	}
	return 0;
}

int main()
{
	Transportation::cout << "Register commands" << Streaming::LF;
	Transportation::command += new Transportation::CommandStop();

	Transportation::cout << "Create console command thread" << Streaming::LF;
	WTM::thread::create(T01C, nullptr);
	return 0;
}

extern "C"
{
void CRTStartup()
{
	Transportation::cout << "CRT Startup successful" << Streaming::LF;
}
}