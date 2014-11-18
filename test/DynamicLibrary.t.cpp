#include "../DynamicLibrary.h"
#include <iostream>
#include <loki/SmartPtr.h>

using namespace PointCX::Plugin::Private;

int main(int argc, char** argv)
{

    typedef Loki::SmartPtr<DynamicLibrary> DynamicLibraryPtr;

    std::string err;
    DynamicLibrary* lib = DynamicLibrary::load(argv[1], err);
    if(!lib)
    {
        std::cout << err << std::endl;
		return -1;
    }
    
	DynamicLibraryPtr d_lib_p(lib);
    typedef int (*AddFuncPtr)(int, int);
    AddFuncPtr ptr = (AddFuncPtr)d_lib_p->getSymbol("SampleAddInt", err);
    if(ptr)
    {
        int ret = ptr(5,2);
        std::cout << ret << std::endl;
    }else
	{
		std::cout << err << std::endl;
		return -1;
	}
    //delete lib;
    return 0;
}
