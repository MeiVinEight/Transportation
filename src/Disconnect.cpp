#include "Disconnect.h"

#include "ConnectionManager.h"
#include "Transportation.h"

Transportation::packet::Disconnect::Disconnect(): Transportation::packet::Datapack(Transportation::packet::Disconnect::ID)
{
}
void Transportation::packet::Disconnect::operator()(Transportation::ConnectionManager &cm) const
{
	String::string str = "[";
	if(this->message)
	{
		str += this->message;
	}
	else
	{
		String::string ip = cm.connection.IP.string();
		if (!cm.connection.IP.IPV4())
		{
			ip = String::string("[") + ip + "]";
		}
		str += ip;
		str += ":";
		str += String::stringify(cm.connection.RP);
	}
	str += "] Disconnect: ";
	str += this->message;
	Transportation::cout << str << Streaming::LF;
	// TODO delete ConnectionManager from NetworkManager
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