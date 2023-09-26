#include <stringstream.h>
#include <format.h>

#include "CommandClose.h"
#include "Transportation.h"
#include "ConnectionManager.h"

Transportation::CommandClose::CommandClose(): Transportation::CMD("close")
{
}
bool Transportation::CommandClose::operator()(const String::string &options) const
{
	Streaming::string string;
	string.address = options.address;
	Streaming::format in(&string);

	String::string name;
	in >> name;
	String::string msg(string.address.address + string.position, string.available());

	Transportation::ConnectionManager *cm = Transportation::network[name];
	if (cm)
	{
		if (!msg)
		{
			msg = "close";
		}
		cm->close(msg);
		(*cm)--;
	}
	else
	{
		Transportation::cout << "Cannot found [" << name << ']' << Streaming::LF;
	}
	return true;
}