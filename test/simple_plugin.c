#include "../plugin.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "simple_object.h"

PLUGIN_API int ExitFunc()
{
    printf("ExitFunc Invoked!\n");
    return 0;
}

PLUGIN_API PF_ExitFunc PF_initPlugin(const PF_PlatformServices *params)
{
    int res = 0;

    PF_RegisterParams rp;
    rp.version.major=1;
    rp.version.minor=9;

    rp.createFunc = simple_create;
    rp.destroyFunc = simple_delete;
    rp.programmingLanguage = PF_ProgrammingLanguage_C;

    printf("simple_create:%p rp.createFunc:%p\n", simple_create, rp.createFunc);

    printf("Registeration for SimpleObject\n");
    res = params->registerObject("SimpleObject", &rp);
    printf("Registeration for SimpleObject Finished, %d\n", res);


    if(res<0)
        return NULL;
    return ExitFunc;
}
