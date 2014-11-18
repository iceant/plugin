#ifndef DYNAMICLIBRARY_H_INCLUDED
#define DYNAMICLIBRARY_H_INCLUDED
#include <string>
namespace PointCX
{
    namespace Plugin
    {
        namespace Private
        {
            class DynamicLibrary
            {
            public:
                static DynamicLibrary* load(const char* libPath, std::string& error);
                ~DynamicLibrary();

                void* getSymbol(const char* name, std::string& error);
				void* getSymbol(const char* name);

            private:
                DynamicLibrary();
                DynamicLibrary(void *handle);
                DynamicLibrary(const DynamicLibrary&);
                DynamicLibrary& operator=(const DynamicLibrary&);

                void* d_handle_p;
            };
        }
    }
}

#endif // DYNAMICLIBRARY_H_INCLUDED
