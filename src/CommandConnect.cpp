#include <stringstream.h>
#include <format.h>
#include <SocketAddress.h>

#include "CommandConnect.h"
#include "Transportation.h"

Transportation::CommandConnect::CommandConnect(): Transportation::CMD("connect")
{
}
bool Transportation::CommandConnect::operator()(const String::string &options) const
{
	Streaming::string string;
	string.address = options.address;
	Streaming::format in(&string);

	String::string ip;
	DWORD port;
	in >> ip >> port;

	WSA::SocketAddress address;
	try
	{
		address = WSA::SocketAddress(WSA::IP((char *) ip.native().address), port);
	}
	catch (Memory::exception &exce)
	{
		Transportation::cout << "Wrong IP: " << ip << Streaming::LF;
		return true;
	}

	try
	{
		Transportation::network.connect(address);
	}
	catch (Memory::exception &exce)
	{
		Transportation::cout << "Cannot connect to " << address.stringify() << Streaming::LF;
		Transportation::cout << exce;
	}
	return true;
}