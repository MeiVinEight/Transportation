#include <endian.h>

#include "ConnectionManager.h"
#include "protocol.h"

Transportation::ConnectionManager::ConnectionManager(WSA::Socket socket): connection((WSA::Socket &&) socket), stream(&this->connection)
{
}
Transportation::ConnectionManager::~ConnectionManager()
{
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