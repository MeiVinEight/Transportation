#include "CommandStop.h"
#include "Transportation.h"

Transportation::CommandStop::CommandStop(): Transportation::CMD("stop")
{
}
bool Transportation::CommandStop::operator()(const String::string &) const
{
	Transportation::network.opening = false;
	Transportation::network.server.close();
	return false;
}