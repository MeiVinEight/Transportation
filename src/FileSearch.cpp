#include <fully.h>
#include <endian.h>
#include <filesystem.h>
#include <filestream.h>
#include <foreach_file.h>

#include "FileSearch.h"
#include "ConnectionManager.h"
#include "FileReply.h"
#include "Transportation.h"

void WriteFile(QWORD prefixLength, const String::string &path, Transportation::ConnectionManager &cm, const String::string &prefix, bool hasMore)
{
	String::string nextPrefix = prefix + (hasMore ? "│ " : "  ");
	if (Filesystem::exist(path))
	{
		// absolute file path
		String::string absolute = Filesystem::canonicalize(path);

		/*---------- PRINT FILE TREE ----------*/
		// last single fs object in absolute path
		String::string parentPath = Filesystem::parent(absolute);
		QWORD parentLength = parentPath.length();
		if (parentLength && parentPath[parentLength - 1] != '\\')
		{
			parentLength++;
		}
		String::string filename(absolute.address.address + parentLength, absolute.length() - parentLength);

		Transportation::cout << prefix << (hasMore ? "├─" : "└─") << filename << Streaming::LF;
		/*---------- PRINT FILE TREE ----------*/

		// relative file path
		String::string relative(absolute.address.address + prefixLength, absolute.length() - prefixLength);

		if (Filesystem::file(absolute))
		{
			// W:\Workspace\C\project\Transportation\.cmake\.cmake\api\v1\reply\target-Transportation-RelWithDebInfo-07b2e8af44772c8e84a6.json
			// open file stream with read access
			Streaming::file file(Filesystem::open(absolute, Filesystem::FA_READ_DATA, Filesystem::FILE_SHARE_READ | Filesystem::FILE_SHARE_WRITE));

			// construct reply packet
			Transportation::packet::FileReply reply;
			reply.name = relative;
			reply.length = file.available();
			reply.more = hasMore;

			// hold output lock
			cm.OL++;

			// send reply
			cm(reply);

			// send file
			Streaming::fully out(&cm.connection);
			QWORD len = reply.length;
			while (len)
			{
				DWORD readed = len > 1024 ? 1024 : len;
				BYTE buf[1024];
				readed = file.read(buf, readed);
				out.write(buf, readed);
				len -= readed;
			}
			file.close();

			// unlock
			cm.OL--;
		}
		else
		{
			// hold output lock
			cm.OL++;

			// reply this is a folder
			Transportation::packet::FileReply reply;
			reply.name = relative;
			reply.length = -2; // -2 identify this is a folder
			reply.more = hasMore;
			cm(reply);

			// foreach dir
			Filesystem::foreach foreach;
			bool search = foreach(absolute);
			while (search)
			{
				String::string childFileName = foreach.name;
				search = foreach();
				if (childFileName != "." && childFileName != "..")
				{
					/*
					* ├─.release
					* │ ├─Concurrency.dll
					* │ └─WTM.dll
					* └─LICENSE
					*/
					String::string childFullPath = absolute;
					if (childFullPath[childFullPath.length() - 1] != '\\')
					{
						childFullPath += '\\';
					}
					childFullPath += childFileName;
					WriteFile(prefixLength, childFullPath, cm, nextPrefix, search);
				}
			}
			foreach.close();

			// reply for the end of the current folder
			reply.name = String::string();
			reply.length = 0;
			reply.more = false;
			cm(reply);

			// unlock
			cm.OL--;
		}
	}
	else
	{
		Transportation::packet::FileReply reply;
		reply.name = path;
		reply.length = -1;
		cm(reply);
	}
}
Transportation::packet::FileSearch::FileSearch(): Transportation::packet::Datapack(FileSearch::ID)
{
}
void Transportation::packet::FileSearch::operator()(Transportation::ConnectionManager &cm)
{
	String::string absolute = Filesystem::canonicalize(this->name);
	String::string parent = Filesystem::parent(absolute);
	Transportation::cout << parent << Streaming::LF;
	QWORD prefixLength = parent.length();
	if (prefixLength && parent[prefixLength - 1] != '\\')
	{
		prefixLength++;
	}
	WriteFile(prefixLength, this->name, cm, "", false);
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
	WORD len = Memory::BE::get((WORD) this->name.length());
	out.write(&len, 2);
	out.write(this->name.address);
	return *this;
}