#include "protocol.h"
#include "datapack.h"
#include "Handshaking.h"
#include "Disconnect.h"

template<typename T>
Transportation::packet::Datapack *ctor()
{
	return (Transportation::packet::Datapack *) (new T);
}

Transportation::packet::Datapack *(*(Transportation::protocol::datapack[]))() = {
	ctor<Transportation::packet::Handshaking>,
	ctor<Transportation::packet::Disconnect>
};
WORD Transportation::protocol::version = 0;