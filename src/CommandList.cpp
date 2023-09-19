#include "CommandList.h"
#include "Transportation.h"

String::string state[] = {"CLOSED", "HANDSHAKING", "PLAYING"};

Transportation::CommandList::CommandList(): Transportation::CMD("list")
{
}
bool Transportation::CommandList::operator()(const String::string &) const
{
	Transportation::NetworkManager &nm = Transportation::network;
	nm.lock++;
	Transportation::cout << nm.length << " connection(s)" << Streaming::LF;
	for (QWORD i = 0; i < nm.length; i++)
	{
		Transportation::ConnectionManager *cm = nm.connection[i];
		Transportation::cout << '[' << cm->name << "] (" << cm->address.stringify() << ") <" << state[cm->opening] << '>' << Streaming::LF;
	}
	nm.lock--;
	return true;
}