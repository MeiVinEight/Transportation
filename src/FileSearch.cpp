#include <fully.h>
#include <endian.h>
#include <filesystem.h>

#include "FileSearch.h"
#include "ConnectionManager.h"
#include "FileReply.h"

Transportation::packet::FileSearch::FileSearch(): Transportation::packet::Datapack(FileSearch::ID)
{
}
void Transportation::packet::FileSearch::operator()(Transportation::ConnectionManager &cm)
{
	if (Filesystem::exist(this->name))
	{
		if (Filesystem::file(this->name))
		{
			// TODO send file
		}
		else
		{
			// TODO send directory
		}
	}
	else
	{
		Transportation::packet::FileReply reply;
		reply.name = this->name;
		reply.length = -1;
		cm(reply);
	}
}
Transportation::packet::FileSearch &Transportation::packet::FileSearch::operator<<(Streaming::stream &stream)
{
	Streaming::fully in(&stream);
	WORD len;
	in.read(&len, 2);
	len = Memory::BE::get(len);
	Memory::string buf(len);
	in.read(buf);
	this->name = String::string(buf);
	return *this;
}
Transportation::packet::FileSearch &Transportation::packet::FileSearch::operator>>(Streaming::stream &stream)
{
	Streaming::fully out(&stream);
	WORD len = Memory::BE::get(this->name.length());
	out.write(&len, 2);
	out.write(this->name.address);
	return *this;
}