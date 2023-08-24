#include "CommandManager.h"

Transportation::CommandManager::CommandManager() = default;
Transportation::CommandManager::~CommandManager()
{
	for (QWORD i = 0; i < this->length; delete this->CMD[i++]);
	delete[] this->CMD;
}
const Transportation::CMD *Transportation::CommandManager::operator[](const String::string &id) const
{
	for (QWORD i = 0; i < this->length; i++)
	{
		if (this->CMD[i]->ID == id)
		{
			return this->CMD[i];
		}
	}
	return nullptr;
}
void Transportation::CommandManager::operator+=(const Transportation::CMD *cmd)
{
	const Transportation::CMD **nCmd = new const Transportation::CMD *[this->length + 1];
	Memory::copy(nCmd, this->CMD, this->length * sizeof(const Transportation::CMD *));
	nCmd[this->length] = cmd;
	delete[] this->CMD;
	this->CMD = nCmd;
	this->length++;
}