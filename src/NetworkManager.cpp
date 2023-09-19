#include <cthread.h>

#include "NetworkManager.h"
#include "Transportation.h"

extern "C"
{
int GetUserNameExA(int, char *, DWORD *);
}

Transportation::NetworkManager::~NetworkManager()
{
	this->close();
	this->length = 0;
	delete[] this->connection;
	this->connection = nullptr;
}
void Transportation::NetworkManager::operator+=(Transportation::ConnectionManager *cm)
{
	this->lock++;
	(*cm)++;
	Transportation::ConnectionManager **conn = new Transportation::ConnectionManager *[this->length + 1];
	Memory::copy(conn, this->connection, sizeof(Transportation::ConnectionManager *) * this->length);
	delete[] this->connection;
	this->connection = conn;
	this->connection[this->length++] = cm;
	this->lock--;
}
void Transportation::NetworkManager::operator-=(Transportation::ConnectionManager *cm)
{
	this->lock++;
	QWORD idx = 0;
	for (QWORD i = 0; i < this->length; i++)
	{
		if (this->connection[i] != cm)
		{
			this->connection[idx++] = this->connection[i];
			continue;
		}
		(*this->connection[i])--;
	}
	this->length = idx;
	this->lock--;
}
Transportation::ConnectionManager *Transportation::NetworkManager::operator[](const String::string &name)
{
	Transportation::ConnectionManager *ret = nullptr;
	this->lock++;
	if (name)
	{
		for (QWORD i = 0; i < this->length; i++)
		{
			if (this->connection[i]->name == name)
			{
				ret = this->connection[i];
			}
		}
	}
	ret ? (*ret)++ : void();
	this->lock--;
	return ret;
}
void Transportation::NetworkManager::operator~()
{
	this->opening = true;

	if (!Transportation::username)
	{
		Memory::string buf(256);
		Memory::fill(buf.address, 0, buf.length);
		DWORD len = buf.length;
		if (!GetUserNameExA(2, (char *) buf.address, &len))
		{
			Transportation::cout << "Cannot get username" << Streaming::LF;

			String::string str = "[";
			DWORD err = Memory::error();
			Transportation::cout << Memory::exception(err, Memory::DOSERROR);
		}

		BYTE *nameBuf = buf.address;
		nameBuf += len;
		while (nameBuf >= buf.address)
		{
			if (*nameBuf == '\\')
			{
				break;
			}
			nameBuf--;
		}
		nameBuf++;
		Transportation::username = String::string(nameBuf, (buf.address + len) - nameBuf);
	}

	Transportation::cout << (String::string("Username: ") + username) << Streaming::LF;

	this->server.bind(WSA::SocketAddress());
	this->server.listen();
	Transportation::cout << (username + " listenng on " + String::stringify(server.LP)) << Streaming::LF;

	while (this->server.opening() && this->opening)
	{
		try
		{
			WSA::Socket socket = this->server.accept();
			Transportation::ConnectionManager *cm = new Transportation::ConnectionManager(this, (WSA::Socket &&) socket);
			DWORD (*lambda)(void *) = [](void *pVoid) -> DWORD
			{
				~*((Transportation::ConnectionManager *) pVoid);
				return 0;
			};
			WTM::thread::create(lambda, cm);

			// socket.close();
		}
		catch (Memory::exception &exce)
		{
			Transportation::cout << exce;
		}
	}

	Transportation::cout << "Network thread exit" << Streaming::LF;
}
void Transportation::NetworkManager::connect(const WSA::SocketAddress &sa)
{
	WSA::Socket socket;
	socket.connect(sa);
	Transportation::ConnectionManager *cm = new Transportation::ConnectionManager(this, (WSA::Socket &&) socket);
	DWORD (*lambda)(void *) = [](void *pVoid) -> DWORD
	{
		~*((Transportation::ConnectionManager *) pVoid);
		return 0;
	};
	WTM::thread::create(lambda, cm);
}
void Transportation::NetworkManager::close()
{
	this->lock++;
	this->opening = false;
	this->server.close();
	Transportation::ConnectionManager **conn = this->connection;
	for (QWORD i = 0; i < this->length; i++)
	{
		conn[i]->close("close");
	}
	this->lock--;
}