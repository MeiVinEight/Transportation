#include <endian.h>

#include "Transportation.h"
#include "NetworkManager.h"
#include "ConnectionManager.h"
#include "protocol.h"
#include "Handshaking.h"
#include "Disconnect.h"

Transportation::ConnectionManager::ConnectionManager(Transportation::NetworkManager *network, WSA::Socket socket):
network(network),
connection((WSA::Socket &&) socket),
stream(&this->connection)
{
}
Transportation::ConnectionManager::~ConnectionManager()
{
	this->network = nullptr;
	this->connection.close();
	this->stream.stream = nullptr;
	this->name = String::string();
}
Transportation::ConnectionManager &Transportation::ConnectionManager::operator>>(Transportation::packet::Datapack *(&datapack))
{
	WORD id = 0;
	this->stream.read(&id, 2);
	id = Memory::BE::get(id);
	datapack = Transportation::protocol::datapack[id]();
	(*datapack) << this->stream;
	return *this;
}
Transportation::packet::Datapack *Transportation::ConnectionManager::operator()()
{
	Transportation::packet::Datapack *datapack = nullptr;
	(*this) >> datapack;
	return datapack;
}
Transportation::ConnectionManager &Transportation::ConnectionManager::operator<<(Transportation::packet::Datapack &datapack)
{
	WORD id = Memory::BE::get(datapack.ID);
	this->stream.write(&id, 2);
	datapack >> this->stream;
	return *this;
}
void Transportation::ConnectionManager::operator()(Transportation::packet::Datapack &datapack)
{
	(*this) << datapack;
}
void Transportation::ConnectionManager::operator~()
{
	WSA::SocketAddress sa(this->connection.IP, this->connection.RP);

	(*this->network) += this;
	try
	{
		Transportation::packet::Datapack *datapack = (*this)();
		if (datapack->ID == Transportation::packet::Disconnect::ID)
		{
			Transportation::packet::Disconnect &disconnect = *((Transportation::packet::Disconnect *) datapack);
			disconnect(*this);
			delete datapack;
			return;
		}
		if (datapack->ID != Transportation::packet::Handshaking::ID)
		{
			delete datapack;
			delete this;
			return;
		}

		Transportation::packet::Handshaking &handshaking = *((Transportation::packet::Handshaking *) datapack);
		if (handshaking.version > Transportation::protocol::version)
		{
			Transportation::packet::Disconnect disconnect;
			disconnect.message = "Unsupported version";
			(*this)(disconnect);
			(*this->network) -= this;
			delete this;
			delete datapack;
			return;
		}
		if (!handshaking.name)
		{
			Transportation::packet::Disconnect disconnect;
			disconnect.message = "Empty username";
			(*this)(disconnect);
			(*this->network) -= this;
			delete this;
			delete datapack;
			return;
		}

		this->name = handshaking.name;
		handshaking.version = Transportation::protocol::version;
		handshaking.name = Transportation::username;
		(*this)(handshaking);
		delete datapack;
	}
	catch (Memory::exception &exce)
	{
		(*this->network) -= this;
		delete this;
		return;
	}

	Transportation::cout << '[' << this->name << "] (" << sa.stringify() << ") joined the communication" << Streaming::LF;

	while (this->connection.opening())
	{
		try
		{
			Transportation::packet::Datapack *datapack = (*this)();
			(*datapack)(*this);
			delete datapack;
		}
		catch (Memory::exception &exce)
		{
			Transportation::cout << exce;
		}
	}

	(*this->network) -= this;
	delete this;
}