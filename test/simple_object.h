#ifndef SIMPLE_OBJEC_H
#define SIMPLE_OBJEC_H
#include "../plugin.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct SimpleObject
{
   char name[255];
   int age;
}SimpleObject;

void* simple_create(PF_ObjectParams* params);

int simple_delete(void* obj);

#endif
