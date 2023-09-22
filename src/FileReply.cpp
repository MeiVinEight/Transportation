#include <fully.h>
#include <endian.h>
#include <filestream.h>

#include "FileReply.h"
#include "Transportation.h"

Transportation::packet::FileReply::FileReply(): Transportation::packet::Datapack(FileReply::ID)
{
}
void Transportation::packet::FileReply::operator()(Transportation::ConnectionManager &cm)
{
	if (this->length != -1)
	{
		Streaming::file file(this->name);
		Streaming::fully out(&file);
		QWORD size = this->length;
		while (size)
		{
			BYTE buf[1024];
			DWORD readed = 1024 < size ? 1024 : size;
			readed = cm.connection.read(buf, readed);
			out.write(buf, readed);
			size -= readed;
		}
		file.flush();
		file.close();
	}
	else
	{
		Transportation::cout << "Cannot found file: " << this->name << Streaming::LF;
	}
}
Transportation::packet::FileReply &Transportation::packet::FileReply::operator<<(Streaming::stream &stream)
{
	Streaming::fully in(&stream);
	WORD len;
	in.read(&len, 2);
	len = Memory::BE::get(len);
	Memory::string buf(len);
	in.read(buf);
	this->name = String::string(buf);
	in.read(&this->length, 8);
	this->length = Memory::BE::get(this->length);
	return *this;
}
Transportation::packet::FileReply &Transportation::packet::FileReply::operator>>(Streaming::stream &stream)
{
	Streaming::fully out(&stream);
	WORD len = Memory::BE::get((WORD) this->name.length());
	out.write(&len, 2);
	out.write(this->name.address);
	QWORD size = Memory::BE::get(this->length);
	out.write(&size, 8);
	return *this;
}