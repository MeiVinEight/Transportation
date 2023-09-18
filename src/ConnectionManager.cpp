#include <endian.h>
#include <cthread.h>

#include "Transportation.h"
#include "NetworkManager.h"
#include "ConnectionManager.h"
#include "protocol.h"
#include "Handshaking.h"
#include "Disconnect.h"
#include "atomic.h"

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
void Transportation::ConnectionManager::operator++(int)
{
	_InterlockedIncrement(&this->waiting);
}
void Transportation::ConnectionManager::operator--(int)
{
	_InterlockedDecrement(&this->waiting);
}
Transportation::ConnectionManager &Transportation::ConnectionManager::operator>>(Transportation::packet::Datapack *(&datapack))
{
	this->IL++;
	WORD id = 0;
	this->stream.read(&id, 2);
	id = Memory::BE::get(id);
	datapack = Transportation::protocol::datapack[id]();
	(*datapack) << this->stream;
	this->IL--;
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
	if ((this->opening != CLOSED) || (datapack.ID == Transportation::packet::Disconnect::ID))
	{
		this->OL++;
		WORD id = Memory::BE::get(datapack.ID);
		this->stream.write(&id, 2);
		datapack >> this->stream;
		this->OL--;
	}
	return *this;
}
void Transportation::ConnectionManager::operator()(Transportation::packet::Datapack &datapack)
{
	(*this) << datapack;
}
void Transportation::ConnectionManager::operator~()
{
	WSA::SocketAddress sa(this->connection.IP, this->connection.RP);
	this->opening = HANDSHAKING;

	(*this->network) += this;
	try
	{
		Transportation::packet::Datapack *datapack = (*this)();
		this->OL++;
		if (datapack->ID == Transportation::packet::Disconnect::ID || datapack->ID == Transportation::packet::Handshaking::ID)
		{
			(*datapack)(*this);
		}
		else
		{
			this->opening = CLOSED;
		}
		delete datapack;

		if (_InterlockedCompareExchange8(&this->opening, PLAYING, HANDSHAKING) == HANDSHAKING)
		{
			Transportation::cout << '[' << this->name << "] (" << sa.stringify() << ") joined the communication" << Streaming::LF;
		}
		this->OL--;
	}
	catch (Memory::exception &exce)
	{
		this->opening = CLOSED;
		while (this->OL)
		{
			this->OL--;
		}
	}

	while (this->opening == PLAYING && this->connection.opening())
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
	this->close("close");
	while (this->waiting)
	{
		WTM::thread::sleep(0);
	}
	delete this;
}
void Transportation::ConnectionManager::close(const String::string &message)
{
	// increment waiting counter
	(*this)++;
	// current thread should handle close signal
	char state = this->opening;
	if ((state == _InterlockedCompareExchange8(&this->opening, CLOSED, state)) && (this->opening == CLOSED))
	{
		// send disconnect to remote
		Transportation::packet::Disconnect disconnect;
		disconnect.message = message;
		// catch and ignore all exceptions
		try
		{
			(*this)(disconnect);
		}
		catch (...)
		{
		}
	}
	// decrement waiting counter
	(*this)--;
}