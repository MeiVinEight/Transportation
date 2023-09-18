#include "datapack.h"
#include "ConnectionManager.h"

Transportation::packet::Datapack::Datapack(WORD id): ID(id)
{
}
Transportation::packet::Datapack::~Datapack() = default;
void Transportation::packet::Datapack::operator()(Transportation::ConnectionManager &)
{
}