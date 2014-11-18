#ifndef HELLO_DLL_H
#define HELLO_DLL_H

#ifdef WIN32
    #ifdef BUILD_DLL
        #ifdef __cplusplus 
            #define DLL_EXPORT extern "C" __declspec(dllexport)  
        #else  
            #define DLL_EXPORT extern __declspec(dllexport)  
        #endif 
    #else  
        #define DLL_EXPORT extern _declspec(dllimport)  
    #endif 
#else
    #define DLL_EXPORT  
#endif //WIN32

DLL_EXPORT int SampleAddInt(int, int);

#endif
