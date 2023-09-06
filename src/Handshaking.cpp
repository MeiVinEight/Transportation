#include <endian.h>

#include "Handshaking.h"
#include "ConnectionManager.h"
#include "Transportation.h"

Transportation::packet::Handshaking::Handshaking(): Transportation::packet::Datapack(Transportation::packet::Handshaking::ID)
{
}
void Transportation::packet::Handshaking::operator()(Transportation::ConnectionManager &cm) const
{
	// String::string str = "[00:00:00:00]:0000 Handshaking V:0000 N:";
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
}
Transportation::packet::Handshaking &Transportation::packet::Handshaking::operator<<(Streaming::stream &stream)
{
	stream.read(&this->version, 4);
	this->version = Memory::BE::get(this->version);

	BYTE length = 0;
	stream.read(&length, 1);
	void *buf = Memory::allocate(length);
	stream.read(buf, length);
	this->name = String::string(buf, length);
	Memory::free(buf);

	return *this;
}
Transportation::packet::Handshaking &Transportation::packet::Handshaking::operator>>(Streaming::stream &stream)
{
	WORD ver = Memory::BE::get(this->version);
	stream.write(&ver, 2);

	BYTE length = this->name.length();
	stream.write(&length, 1);
	stream.write(this->name.address.address, length);
	return *this;
}