#include <stream.h>

#include "protocol.h"
#include "datapack.h"

template<typename T>
Transportation::packet::Datapack *ctor()
{
	return (Transportation::packet::Datapack *) (new T);
}

Transportation::packet::Datapack *(*(Transportation::protocol::datapack[1]))() = {
};