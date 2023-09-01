#include <sstring.h>
#include <standardstream.h>
#include <cthread.h>
#include <exception.h>
#include <hexadecimal.h>
#include <endian.h>
#include <Socket.h>

#include "Transportation.h"
#include "CMD.h"
#include "CommandStop.h"
#include "NetworkManager.h"

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
Transportation::NetworkManager network;
extern "C"
{
int GetUserNameExA(int, char *, DWORD *);
}
DWORD T02C(void *)
{
	String::string name;

	{
		Memory::string buf(256);
		Memory::fill(buf.address, 0, buf.length);
		DWORD length = buf.length;
		if (!GetUserNameExA(2, (char *) buf.address, &length))
		{
			Transportation::cout << "Cannot get username";

			String::string str = "[";
			DWORD err = Memory::error();
			Memory::string x(4);
			Memory::BE::set(err, x.address, 4);
			str += Hexadecimal::format(x);
			str += "] ";
			str += Memory::message(err, Memory::DOSERROR);
			Transportation::cout << str;
			return err;
		}

		BYTE *nameBuf = buf.address;
		nameBuf += length;
		while (nameBuf >= buf.address)
		{
			if (*nameBuf == '\\')
			{
				break;
			}
			nameBuf--;
		}
		nameBuf++;
		name = String::string(nameBuf, (buf.address + length) - nameBuf);
	}

	Transportation::cout << (String::string("Username: ") + name);

	WSA::Socket server;
	server.bind(WSA::SocketAddress(0));
	server.listen();
	Transportation::cout << (name + " listenng on " + String::stringify(server.LP));

	while (server.opening())
	{
		WSA::Socket socket = server.accept();
		String::string ip = socket.IP.string();
		if (!socket.IP.IPV4())
		{
			ip = String::string("[") + ip + "]";
		}
		ip += ':';
		ip += String::stringify(socket.RP);
		Transportation::cout << (String::string("Connection: ") + ip);

		socket.close();
	}

	return 0;
}

int main()
{
	Transportation::cout << "Register commands";
	Transportation::command += new Transportation::CommandStop();

	Transportation::cout << "Create console command thread";
	WTM::thread::create(T01C, nullptr);

	Transportation::cout << "Create connection listener";
	WTM::thread::create(T02C, nullptr);

	return 0;
}

extern "C"
{
void CRTStartup()
{
	Transportation::cout << "CRT Startup successful";
}
}