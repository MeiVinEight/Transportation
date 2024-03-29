#include <sstring.h>
#include <standardstream.h>
#include <cthread.h>
#include <exception.h>
#include <streaming.h>
#include <stringstream.h>

#include "Transportation.h"
#include "CMD.h"
#include "CommandStop.h"
#include "CommandConnect.h"
#include "CommandClose.h"
#include "CommandList.h"
#include "CommandSearch.h"

DWORD T01C(void *)
{
	bool running = true;
	while (running)
	{
		Transportation::cout.lock();
		Transportation::cout.Streaming::format::write("> ", 2);
		Transportation::cout.unlock();

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
	Transportation::cout << "Command thread exit" << Streaming::LF;
	return 0;
}
DWORD T02C(void *p)
{
	~*((Transportation::NetworkManager *) p);
	return 0;
}

int main()
{
	Transportation::cout << "Register commands" << Streaming::LF;
	Transportation::command += new Transportation::CommandStop();
	Transportation::command += new Transportation::CommandConnect();
	Transportation::command += new Transportation::CommandClose();
	Transportation::command += new Transportation::CommandList();
	Transportation::command += new Transportation::CommandSearch();

	Transportation::cout << "Create console command thread" << Streaming::LF;
	WTM::thread::create(T01C, nullptr);

	Transportation::cout << "Create connection listener" << Streaming::LF;
	WTM::thread::create(T02C, &Transportation::network);

	Transportation::cout << "Main thread exit" << Streaming::LF;

	return 0;
}

extern "C"
{
void CRTStartup()
{
	Transportation::cout << "CRT Startup successful" << Streaming::LF;
}
}