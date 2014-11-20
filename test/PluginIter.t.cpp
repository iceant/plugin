#include "../PluginIter.h"
#include <iostream>

using namespace PointCX::Plugin::Private;

int main(int argc, char** argv)
{
    for(PluginIter iter(argv[1]); iter; ++iter)
    {
        std::cout << iter() << std::endl;
    }
}
