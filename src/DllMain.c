int main();
void CRTStartup(void);

#ifndef CMAKE_BUILD

#pragma section(".CRT$XCA", read)
#pragma section(".CRT$XCZ", read)
#pragma comment(linker, "/MERGE:.CRT=.rdata")

__declspec(allocate(".CRT$XCA")) void (__cdecl *XCA)() = 0;
__declspec(allocate(".CRT$XCZ")) void (__cdecl *XCZ)() = 0;

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
	CRTINIT();
	return main();
}

#endif