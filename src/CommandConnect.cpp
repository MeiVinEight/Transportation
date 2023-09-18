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
	WSA::SocketAddress address(WSA::IP((char *) ip.native().address), port);
	Transportation::cout << "Connect to " << address.stringify() << Streaming::LF;
	Transportation::network.connect(address);
	return true;
}