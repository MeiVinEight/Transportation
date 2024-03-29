#include <endian.h>

#include "Handshaking.h"
#include "ConnectionManager.h"
#include "Transportation.h"
#include "protocol.h"

Transportation::packet::Handshaking::Handshaking(): Transportation::packet::Datapack(Transportation::packet::Handshaking::ID)
{
}
void Transportation::packet::Handshaking::operator()(Transportation::ConnectionManager &cm)
{
	// String::string str = "[00:00:00:00]:0000 Handshaking V:0000 N:";
	/*
	String::string str = cm.connection.IP.string();
	if (!cm.connection.IP.IPV4())
	{
		str = String::string("[") + str + "]";
	}
	str += ":";
	str += String::stringify(cm.connection.RP);
	str += " Handshaking V:";
	str += String::stringify(this->version);
	str += " N:";
	str += this->name;
	Transportation::cout << str << Streaming::LF;
	*/
	cm.OL++;
	Transportation::ConnectionManager *cms;
	if (this->version > Transportation::protocol::version)
	{
		cm.close("Unsupported version");
		goto END;
	}
	if (!this->name)
	{
		cm.close("Empty username");
		goto END;
	}
	if ((cms = (*cm.network)[this->name]))
	{
		(*cms)--;
		cm.close("Username already exists");
		goto END;
	}
	cm.name = this->name;
	END:;
	cm.OL--;
}
Transportation::packet::Handshaking &Transportation::packet::Handshaking::operator<<(Streaming::stream &stream)
{
	Streaming::fully in(&stream);
	in.read(&this->version, 2);
	this->version = Memory::BE::get(this->version);

	BYTE length = 0;
	in.read(&length, 1);
	void *buf = Memory::allocate(length);
	in.read(buf, length);
	this->name = String::string(buf, length);
	Memory::free(buf);

	return *this;
}
Transportation::packet::Handshaking &Transportation::packet::Handshaking::operator>>(Streaming::stream &stream)
{
	Streaming::fully out(&stream);
	WORD ver = Memory::BE::get(this->version);
	out.write(&ver, 2);

	BYTE length = this->name.length();
	out.write(&length, 1);
	out.write(this->name.address.address, length);
	return *this;
}