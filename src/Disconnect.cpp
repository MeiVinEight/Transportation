#include "Disconnect.h"

#include "ConnectionManager.h"
#include "Transportation.h"

Transportation::packet::Disconnect::Disconnect(): Transportation::packet::Datapack(Transportation::packet::Disconnect::ID)
{
}
void Transportation::packet::Disconnect::operator()(Transportation::ConnectionManager &cm)
{
	if (cm.name)
	{
		Transportation::cout << '[' << cm.name << "] ";
	}
	else
	{
		Transportation::cout << '(' << WSA::SocketAddress(cm.connection.IP, cm.connection.RP).stringify() << ") ";
	}
	Transportation::cout << "Disconnect: " << this->message << Streaming::LF;
	cm.close(this->message);
}
Transportation::packet::Disconnect &Transportation::packet::Disconnect::operator<<(Streaming::stream &stream)
{
	Streaming::fully in(&stream);
	BYTE length = 0;
	in.read(&length, 1);
	void *buf = Memory::allocate(length);
	in.read(buf, length);
	this->message = String::string(buf, length);
	Memory::free(buf);
	return *this;
}
Transportation::packet::Disconnect &Transportation::packet::Disconnect::operator>>(Streaming::stream &stream)
{
	Streaming::fully out(&stream);
	BYTE length = this->message.length();
	out.write(&length, 1);
	out.write(this->message.address.address, length);
	return *this;
}