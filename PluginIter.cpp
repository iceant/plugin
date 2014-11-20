#include "PluginIter.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include <vector>

namespace bf = boost::filesystem;

namespace PointCX
{
    namespace Plugin
    {
        namespace Private
        {
            class PluginIter_i
            {
                typedef std::vector<bf::path> PluginPathVec;

                public:
                    bf::path root;
                    PluginPathVec d_paths;
                    size_t index;

                    PluginIter_i(const char* path)
                        :root(path)
                         ,index(0)
                    {
                    }

                    int load(){
                        bf::directory_iterator end_iter;
                        for(bf::directory_iterator it(root); it!=end_iter; it++)
                        {
#if defined(_WIN32)
                            if(bf::exists(it->path()/"plugin.dll"))
                               d_paths.push_back(it->path()/"plugin.dll");
#elif defined(__unix__)
                            if(bf::exists(it->path()/"plugin.so"))
                               d_paths.push_back(it->path()/"plugin.so");
#elif defined(__APPLE__)
                            if(bf::exists(it->path()/"plugin.dylib"))
                               d_paths.push_back(it->path()/"plugin.dylib");
#endif
                        }
                        return d_paths.size();
                    }
            };


            PluginIter::PluginIter(const char* path)
                :d_this(new PluginIter_i(path))
            {
                d_this->load();
            }

            PluginIter::~PluginIter()
            {
                if(d_this)
                {
                    delete d_this;
                    d_this=NULL;
                }
            }

            void PluginIter::operator++()
            {
                d_this->index++;
            }

            // for(;it;)
            PluginIter::operator const void*() const
            {
                if(d_this->index < d_this->d_paths.size())
                    return this;
                return NULL;
            }

            // it()
            const char* PluginIter::operator()() const
            {
                return bf::absolute(d_this->d_paths[d_this->index]).c_str();
            }
        }
    }
}
