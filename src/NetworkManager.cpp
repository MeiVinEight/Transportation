#include "NetworkManager.h"

Transportation::NetworkManager::~NetworkManager()
{
	this->length = 0;
	for (QWORD i = 0; i < this->length; i++)
	{
		delete this->connection[i];
	}
	delete[] this->connection;
}
void Transportation::NetworkManager::operator+=(const WSA::Socket &socket)
{
	Transportation::ConnectionManager **conn = new Transportation::ConnectionManager *[this->length + 1];
	Memory::copy(conn, this->connection, sizeof(Transportation::ConnectionManager *) * this->length);
	this->connection[this->length++] = new Transportation::ConnectionManager((WSA::Socket &&) socket);
	delete[] this->connection;
	this->connection = conn;
}
Transportation::ConnectionManager *Transportation::NetworkManager::operator[](const String::string &name) const
{
	if (name)
	{
		for (QWORD i = 0; i < this->length; i++)
		{
			if (this->connection[i]->name == name)
			{
				return this->connection[i];
			}
		}
	}
	return nullptr;
}