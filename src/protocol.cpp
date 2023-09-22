#include "protocol.h"
#include "datapack.h"
#include "Handshaking.h"
#include "Disconnect.h"
#include "FileSearch.h"
#include "FileReply.h"

template<typename T>
Transportation::packet::Datapack *ctor()
{
	return (Transportation::packet::Datapack *) (new T);
}

Transportation::packet::Datapack *(*(Transportation::protocol::datapack[]))() = {
	ctor<Transportation::packet::Handshaking>,
	ctor<Transportation::packet::Disconnect>,
	ctor<Transportation::packet::FileSearch>,
	ctor<Transportation::packet::FileReply>,
};
WORD Transportation::protocol::version = 0;