#include "../plugin.h"
#include "simple_object.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* simple_create(PF_ObjectParams* params)
{
    printf("simple_create ...\n");
    if(!params)
    {
        printf("Invalid Parameter\n");
        return NULL;
    }
    printf("simple_create->Check objectType[%s] with SimpleObject\n", params->objectType);
    if(strcmp(params->objectType, "SimpleObject")==0)
    {
        SimpleObject * obj= (SimpleObject*)malloc(sizeof(SimpleObject));
        return obj;
    }
    printf("Wrong objectType[%s]\n", params->objectType);
    return NULL;
}

int simple_delete(void* obj)
{
    printf("simple_delete...%p\n", obj);
    if(!obj)
        return -1;
    free((SimpleObject*)obj);
	printf("simple_delete...FINISHED\n");
    return 0;
}
