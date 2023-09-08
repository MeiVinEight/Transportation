#include <sstring.h>
#include <standardstream.h>
#include <cthread.h>
#include <exception.h>
#include <Socket.h>
#include <streaming.h>
#include <stringstream.h>

#include "Transportation.h"
#include "CMD.h"
#include "CommandStop.h"
#include "NetworkManager.h"
#include "protocol.h"
#include "Handshaking.h"
#include "Disconnect.h"

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
extern "C"
{
int GetUserNameExA(int, char *, DWORD *);
}
DWORD T02C(void *p)
{
	String::string username;
	Transportation::NetworkManager &network = *((Transportation::NetworkManager *) p);

	{
		Memory::string buf(256);
		Memory::fill(buf.address, 0, buf.length);
		DWORD length = buf.length;
		if (!GetUserNameExA(2, (char *) buf.address, &length))
		{
			Transportation::cout << "Cannot get username" << Streaming::LF;

			String::string str = "[";
			DWORD err = Memory::error();
			Transportation::cout << Memory::exception(err, Memory::DOSERROR);
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
		username = String::string(nameBuf, (buf.address + length) - nameBuf);
	}

	Transportation::cout << (String::string("Username: ") + username) << Streaming::LF;

	WSA::Socket &server = Transportation::network.server;
	server.bind(WSA::SocketAddress(12138));
	server.listen();
	Transportation::cout << (username + " listenng on " + String::stringify(server.LP)) << Streaming::LF;

	while (server.opening() && network.opening)
	{
		try
		{
			WSA::Socket socket = server.accept();
			Transportation::ConnectionManager *cm = new Transportation::ConnectionManager((WSA::Socket &&) socket);
			Transportation::network += cm;

			Transportation::packet::Datapack *datapack = (*cm)();
			if (datapack->ID == Transportation::packet::Disconnect::ID)
			{
				Transportation::packet::Disconnect &disconnect = *((Transportation::packet::Disconnect *) datapack);
				disconnect(*cm);
				delete datapack;
				continue;
			}
			if (datapack->ID != Transportation::packet::Handshaking::ID)
			{
				delete datapack;
				delete cm;
				continue;
			}

			Transportation::packet::Handshaking &handshaking = *((Transportation::packet::Handshaking *) datapack);
			if (handshaking.version > Transportation::protocol::version)
			{
				Transportation::packet::Disconnect disconnect;
				disconnect.message = "Unsupported version";
				(*cm)(disconnect);
				Transportation::network -= cm;
				delete cm;
				delete datapack;
				continue;
			}
			if (!handshaking.name)
			{
				Transportation::packet::Disconnect disconnect;
				disconnect.message = "Empty username";
				(*cm)(disconnect);
				Transportation::network -= cm;
				delete cm;
				delete datapack;
				continue;
			}

			cm->name = handshaking.name;
			Transportation::cout << cm->name << " (" << WSA::SocketAddress(cm->connection.IP, cm->connection.RP).stringify() << ") joined the communication" << Streaming::LF;
			handshaking.version = Transportation::protocol::version;
			handshaking.name = username;
			(*cm)(handshaking);
			delete datapack;

			// socket.close();
		}
		catch (Memory::exception &exce)
		{
			Transportation::cout << exce;
		}
	}

	Transportation::cout << "Network thread exit" << Streaming::LF;
	return 0;
}

int main()
{
	Transportation::cout << "Register commands" << Streaming::LF;
	Transportation::command += new Transportation::CommandStop();

	Transportation::cout << "Create console command thread" << Streaming::LF;
	WTM::thread::create(T01C, nullptr);

	Transportation::network.opening = true;
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