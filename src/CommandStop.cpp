#include "CommandStop.h"

Transportation::CommandStop::CommandStop(): Transportation::CMD("stop")
{
}
bool Transportation::CommandStop::operator()(const String::string &) const
{
	return false;
}