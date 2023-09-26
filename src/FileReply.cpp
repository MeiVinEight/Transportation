#include <fully.h>
#include <endian.h>
#include <filestream.h>
#include <timestamp.h>

#include "FileReply.h"
#include "Transportation.h"

String::string FormatSize(QWORD size)
{
	char unit[][3] = {" B", "KB", "MB", "GB"};
	double fSize = (double) size;
	BYTE unitUse = 0;
	while (fSize >= 1024)
	{
		fSize /= 1024;
		unitUse++;
	}
	String::string ret = String::stringify(fSize, 2);
	ret += unit[unitUse];
	return ret;
}
void Speed(QWORD totalLength, QWORD size, QWORD speed, QWORD ms)
{
	ms += !ms;
	speed *= 1000;
	speed /= ms;
	QWORD time = 100 * 3600;
	if (speed)
		time = size / speed;

	String::string info = " ";
	if (time >= 100 * 3600)
	{
		info += "99:99:99";
	}
	else
	{
		QWORD ss = time % 60;
		time /= 60;
		QWORD mm = time % 60;
		time /= 60;
		QWORD hh = time;
		if (hh < 10) info += '0';
		info += String::stringify(hh);
		info += ':';
		if (mm < 10) info += '0';
		info += String::stringify(mm);
		info += ':';
		if (ss < 10) info += '0';
		info += String::stringify(ss);
		info += ' ';
	}

	info += FormatSize(speed);
	info += "/s ";
	info += FormatSize(totalLength - size);
	info += '/';
	info += FormatSize(totalLength);

	Transportation::cout.Streaming::format::write(info.address.address, info.length());
	String::string vts = "\x1B[0K\x1B[";
	vts += String::stringify(info.length());
	vts += 'D';
	Transportation::cout.Streaming::format::write(vts.address.address, vts.length());
}
void ReadFile(Transportation::ConnectionManager &cm, Transportation::packet::FileReply &reply, const String::string &prefix)
{
	String::string absolutePath = Filesystem::canonicalize(reply.name);
	String::string parentPath = Filesystem::parent(absolutePath);
	QWORD parentLength = parentPath.length();
	parentLength += absolutePath[parentLength] == '\\';
	String::string filename(absolutePath.address.address + parentLength, absolutePath.length() - parentLength);
	String::string nextPrefix = prefix + (reply.more ? "│ " : "  ");

	switch (reply.length)
	{
		case (~(QWORD) 0) - 0: // -1
		{
			Transportation::cout << "Cannot found file: " << reply.name << Streaming::LF;
			break;
		}
		case (~(QWORD) 0) - 1: // -2
		{
			Transportation::cout << prefix << (reply.more ? "├─" : "└─") << filename << Streaming::LF;
			cm.IL++;

			Filesystem::make(absolutePath);

			Transportation::packet::FileReply *child = (Transportation::packet::FileReply *) cm();
			while (child->name)
			{
				ReadFile(cm, *child, nextPrefix);
				delete child;
				child = (Transportation::packet::FileReply *) cm();
			}
			cm.IL--;
			break;
		}
		default:
		{
			Transportation::cout.lock();
			// Transportation::cout << prefix << (reply.more ? "├─" : "└─") << filename << Streaming::LF;
			Transportation::cout.Streaming::format::write("\u001B7\n\u001B8\x1B[1B\u001B7\x1B[1A\x1B[1G\x1B[1L", 23);
			Transportation::cout.Streaming::format::write(Transportation::streaming::time().address, 11);
			String::string fileTree = prefix + (reply.more ? "├─" : "└─") + filename;
			Transportation::cout.Streaming::format::write(fileTree.address.address, fileTree.length());
			try
			{
				Filesystem::create(reply.name);
				Streaming::file file(reply.name);
				Streaming::fully out(&file);
				QWORD size = reply.length;


				QWORD speed = 0;


				QWORD beg = Timestamp::current();
				Speed(reply.length, size, speed, 0);
				while (size)
				{
					BYTE buf[1024];
					DWORD readed = 1024 < size ? 1024 : size;
					readed = cm.connection.read(buf, readed);
					out.write(buf, readed);
					size -= readed;

					speed += readed;
					QWORD end = Timestamp::current();
					if (end - beg >= 50)
					{
						Speed(reply.length, size, speed, end - beg);
						beg = end;
						speed = 0;
					}
				}
				Speed(reply.length, size, speed, Timestamp::current() - beg);
				file.flush();
				file.close();
			}
			catch (Memory::exception &exce)
			{
				Transportation::cout.Streaming::format::write(" ", 1);
				Transportation::cout.Streaming::format::write(exce.message.address, exce.message.length);
				// TODO print
			}
			Transportation::cout.Streaming::format::write("\x1B[0K\u001B8", 6);
			Transportation::cout.unlock();
		}
	}
}
Transportation::packet::FileReply::FileReply(): Transportation::packet::Datapack(FileReply::ID)
{
}
void Transportation::packet::FileReply::operator()(Transportation::ConnectionManager &cm)
{
	Transportation::cout << Filesystem::canonicalize(".") << Streaming::LF;
	ReadFile(cm, *this, "");
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
	in.read(&this->more, 1);
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
	out.write(&this->more, 1);
	return *this;
}