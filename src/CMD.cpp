#include "CMD.h"

Transportation::CMD::CMD(String::string id): ID((String::string &&) (id))
{
}
Transportation::CMD::~CMD() = default;