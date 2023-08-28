#include "protocol.h"
#include "datapack.h"

template<typename T>
Transportation::packet::Datapack *ctor(BYTE *data)
{
	return (Transportation::packet::Datapack *) (new T(data));
}

Transportation::packet::Datapack *(*(Transportation::protocol::datapack[]))(BYTE *) = {
};