#ifndef HELLO_DLL_H
#define HELLO_DLL_H

#ifdef WIN32
#ifdef BUILD_DLL
    #define DLL_EXPORT extern "C" __declspec(dllexport)
#else
    #define DLL_EXPORT extern "C" __declspec(dllimport)
#endif
#endif //WIN32

DLL_EXPORT int SampleAddInt(int, int);

#endif
