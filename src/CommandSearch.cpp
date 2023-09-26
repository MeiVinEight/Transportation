#include <stringstream.h>
#include <format.h>

#include "CommandSearch.h"
#include "Transportation.h"
#include "FileSearch.h"

Transportation::CommandSearch::CommandSearch(): Transportation::CMD("search")
{
}
bool Transportation::CommandSearch::operator()(const String::string &options) const
{
	Streaming::string stream;
	stream.address = options.address;
	Streaming::format opt(&stream);

	String::string conn;
	opt >> conn;
	String::string file(stream.address.address + stream.position, stream.available());
	if (!conn)
	{
		Transportation::cout << "Missing connection name" << Streaming::LF;
		return true;
	}
	if (!file)
	{
		Transportation::cout << "Missing file path" << Streaming::LF;
		return true;
	}

	Transportation::ConnectionManager *cm = Transportation::network[conn];
	if (!cm)
	{
		Transportation::cout << "Cannot found connection [" << conn << ']' << Streaming::LF;
		return true;
	}

	Transportation::packet::FileSearch search;
	search.name = file;
	(*cm)(search);
	(*cm)--;

	return true;
}
