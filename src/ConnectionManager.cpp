#include "ConnectionManager.h"

Transportation::ConnectionManager::ConnectionManager(WSA::Socket socket): connection((WSA::Socket &&) socket)
{
}