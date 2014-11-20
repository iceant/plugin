#ifndef POINTCX_PLUGIN_MANAGER_H
#define POINTCX_PLUGIN_MANAGER_H

#include "plugin.h"

namespace PointCX
{
    namespace Plugin
    {
        class IObjectAdapter;

        class PluginManager_i;

        class PluginManager
        {
            private:
                PluginManager_i *d_this;

                PluginManager(const PluginManager&);
                PluginManager& operator=(const PluginManager&);
            public:
                PluginManager();
                ~PluginManager();

            public:
                static PluginManager& getInstance();

                static int initializePlugin(PF_InitFunc initFunc);

                int loadAll(const char* path, PF_InvokeServiceFunc func = 0);

                int load(const char* path);

                void* createObject(const char* objectType, IObjectAdapter &adapter);

                int shutdown();

                static int registerObject(const char* type, const PF_RegisterParams *params);

                PF_PlatformServices & getPlatformServices();

        };
    }
}
#endif
