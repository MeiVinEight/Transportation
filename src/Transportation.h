#ifndef TRANSPORTATION_TRANSPORTATION_H
#define TRANSPORTATION_TRANSPORTATION_H

#include <standardstream.h>

#include "CommandManager.h"
#include "Tstreaming.h"

namespace Transportation
{
	Transportation::CommandManager command;
	Transportation::streaming cout(&Streaming::cout);
}

#endif //TRANSPORTATION_TRANSPORTATION_H
