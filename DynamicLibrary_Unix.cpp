#include "DynamicLibrary.h"
#include <dlfcn.h>

namespace PointCX
{
    namespace Plugin
    {
        namespace Private
        {

            DynamicLibrary::DynamicLibrary(void* handle)
            :d_handle_p(handle)
            {}

            DynamicLibrary::~DynamicLibrary()
            {
                if(d_handle_p)
                {
                    dlclose(d_handle_p);
                    d_handle_p = NULL;
                }
            }

            DynamicLibrary* DynamicLibrary::load(const char* libPath,
                                 std::string& error)
            {
                if(libPath==NULL || ::strlen(libPath)==0)
                {
                    error = "Error library path.";
                    return NULL;
                }

                void* handle;
                //handle = dlopen(libPath, RTLD_NOW);
                handle = dlopen(libPath, RTLD_LAZY);
                if(!handle)
                {
                    error = dlerror();
                    return NULL;
                }
                return new DynamicLibrary(handle);
            }

            void* DynamicLibrary::getSymbol(const char* symbol)
            {
                if(!d_handle_p)
                    return NULL;

                return dlsym(d_handle_p, symbol);
            }

            void* DynamicLibrary::getSymbol(const char* symbol, std::string &err)
            {
                if(!d_handle_p)
                    return NULL;

                if(!symbol || strlen(symbol)==0)
                {
                    return NULL;
                }

                void* p = dlsym(d_handle_p, symbol);
                if(!p){
                    err = dlerror();
                    return NULL;
                }
                return p;

            }
        }
    }
}
