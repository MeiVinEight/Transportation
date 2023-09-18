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
	BYTE length = 0;
	stream.read(&length, 1);
	void *buf = Memory::allocate(length);
	stream.read(buf, length);
	this->message = String::string(buf, length);
	Memory::free(buf);
	return *this;
}
Transportation::packet::Disconnect &Transportation::packet::Disconnect::operator>>(Streaming::stream &stream)
{
	BYTE length = this->message.length();
	stream.write(&length, 1);
	stream.write(this->message.address.address, length);
	return *this;
}