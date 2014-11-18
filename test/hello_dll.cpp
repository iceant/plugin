#include "hello_dll.h"
#ifdef WIN32
#include <windows.h>
#endif

DLL_EXPORT int SampleAddInt(int x, int y)
{
	return x+y;
}
