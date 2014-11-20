#ifndef POINTCX_PLUGIN_OBJADAPTER_H
#define POINTCX_PLUGIN_OBJADAPTER_H
#include "plugin.h"

namespace PointCX
{
    namespace Plugin
    {
        struct IObjectAdapter
        {
            virtual ~IObjectAdapter(){}

            virtual void* adapt(void* object, PF_DestroyFunc df) = 0;
        };


        template<typename T, typename U>
            struct ObjectAdapter: public IObjectAdapter{
                virtual void* adapt(void* object, PF_DestroyFunc df)
                {
                    return new T((U*)object, df);
                }
            }
        ;
    }
}

#endif
