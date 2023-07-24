#include <standardstream.h>
#include <streaming.h>

int main()
{
	Streaming::cout << "Hello, World!" << Streaming::LF;
	return 0;
}

#ifndef CMAKE_BUILD
int mainCRTStartup()
{
	return main();
}
#endif