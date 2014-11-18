#include "DynamicLibrary.h"
#include <Windows.h>
#include <sstream>

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
                    ::FreeLibrary((HMODULE)d_handle_p);
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
                handle = ::LoadLibraryA(libPath);
                if(handle==NULL)
                {
                    DWORD errorCode = ::GetLastError();
                    std::stringstream ss;
                    ss << "LoadLibrary(" << libPath
                       << ") Failed. Error Code: "
                       << errorCode;
                    error = ss.str();
                    return NULL;
                }
                return new DynamicLibrary(handle);
            }

			void* DynamicLibrary::getSymbol(const char* symbol)
            {
				 if(!d_handle_p)
                    return NULL;

                if(symbol==NULL || ::strlen(symbol)==0)
                {
                    return NULL;
                }

                return (void*)::GetProcAddress((HMODULE)d_handle_p, symbol);
			}
			
            void* DynamicLibrary::getSymbol(const char* symbol, std::string& error)
            {
                if(!d_handle_p)
                    return NULL;

                if(symbol==NULL || ::strlen(symbol)==0)
                {
                    return NULL;
                }

                void* func_ptr = (void*)::GetProcAddress((HMODULE)d_handle_p, symbol);
				if(!func_ptr){
					DWORD errorCode = ::GetLastError();
                    std::stringstream ss;
                    ss << "Get Symbol(" << symbol
                       << ") Failed. Error Code: "
                       << errorCode;
                    error = ss.str();
                    return NULL;
				}
				return func_ptr;
            }
        }
    }
}
