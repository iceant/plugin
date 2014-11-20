#ifndef POINTCX_PLUGIN_ITER_HEAD
#define POINTCX_PLUGIN_ITER_HEAD
namespace PointCX
{
    namespace Plugin
    {
        namespace Private
        {
            class PluginIter_i;

            class PluginIter
            {
                private:
                    PluginIter_i *d_this;

                    PluginIter(const PluginIter&);
                    PluginIter& operator=(const PluginIter&);

                public:
                    PluginIter(const char* path);
                    ~PluginIter();

                    void operator++();

                    operator const void*() const;
                        //Return none-zeor if the iteration is valid. otherwise 0.
                        
                    const char* operator()() const;
                        //Return the value of the current plugin path

            };
        }
    }
}
#endif
