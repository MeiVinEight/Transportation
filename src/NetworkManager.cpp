#include "NetworkManager.h"
#include "Transportation.h"

Transportation::NetworkManager::~NetworkManager()
{
	this->opening = false;
	this->server.close();
	for (QWORD i = 0; i < this->length; i++)
	{
		delete this->connection[i];
	}
	this->length = 0;
	delete[] this->connection;
	this->connection = nullptr;
}
void Transportation::NetworkManager::operator+=(Transportation::ConnectionManager *cm)
{
	Transportation::ConnectionManager **conn = new Transportation::ConnectionManager *[this->length + 1];
	Memory::copy(conn, this->connection, sizeof(Transportation::ConnectionManager *) * this->length);
	delete[] this->connection;
	this->connection = conn;
	this->connection[this->length++] = cm;
	Transportation::cout << cm->name << " (" << WSA::SocketAddress(cm->connection.IP, cm->connection.RP).stringify() << " ) joined the communication" << Streaming::LF;
}
void Transportation::NetworkManager::operator-=(Transportation::ConnectionManager *cm)
{
	QWORD newLen = this->length - !!this->length;
	Transportation::ConnectionManager **conn = new Transportation::ConnectionManager *[newLen];
	QWORD idx = 0;
	for (QWORD i = 0; i < this->length; i++)
	{
		if (this->connection[i] != cm)
		{
			conn[idx++] = this->connection[i];
		}
	}
	this->length = newLen;
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