#include <stream.h>

#include "protocol.h"
#include "datapack.h"

template<typename T>
Transportation::packet::Datapack *ctor(Streaming::stream &stream)
{
	return (Transportation::packet::Datapack *) (new T(stream));
}

Transportation::packet::Datapack *(*(Transportation::protocol::datapack[]))(Streaming::stream &) = {
};