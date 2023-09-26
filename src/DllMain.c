int main();
void CRTStartup(void);

#ifndef CMAKE_BUILD0

#include <WinType.h>

int _fltused = 1;
#pragma section(".CRT$XCA", read)
#pragma section(".CRT$XCZ", read)
#pragma comment(linker, "/MERGE:.CRT=.rdata")

__declspec(allocate(".CRT$XCA")) void (__cdecl *XCA)() = 0;
__declspec(allocate(".CRT$XCZ")) void (__cdecl *XCZ)() = 0;

int __stdcall GetConsoleMode(void *, DWORD *);
int __stdcall SetConsoleMode(void *, DWORD);
void *__stdcall GetStdHandle(DWORD);

void CRTINIT()
{
	for (void (__cdecl **xca)() = &XCA; xca < &XCZ; xca++)
	{
		if (*xca)
		{
			(*xca)();
		}
	}
	CRTStartup();
}

int mainCRTStartup()
{
	void *outputHandle = GetStdHandle(-11);
	DWORD outputMode = 0;
	GetConsoleMode(outputHandle, &outputMode);
	SetConsoleMode(outputHandle, outputMode | 5);
	CRTINIT();
	return main();
}

#endif