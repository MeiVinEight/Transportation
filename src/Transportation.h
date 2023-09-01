#ifndef TRANSPORTATION_TRANSPORTATION_H
#define TRANSPORTATION_TRANSPORTATION_H

#include <standardstream.h>

#include "CommandManager.h"
#include "Tstreaming.h"
#include "NetworkManager.h"

namespace Transportation
{
	extern Transportation::CommandManager command;
	extern Transportation::streaming cout;
	extern Transportation::NetworkManager network;
}

#endif //TRANSPORTATION_TRANSPORTATION_H
