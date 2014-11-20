#include "PluginManager.h"
#include "PluginIter.h"
#include "DynamicLibrary.h"
#include "ObjectAdapter.h"
#include <loki/SmartPtr.h>
#include <loki/Singleton.h>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <map>
#include <vector>
#include <iostream>

namespace bf = boost::filesystem;
namespace pcx = PointCX::Plugin::Private;

namespace PointCX
{
    namespace Plugin
    {
        struct PluginManagerHolder
        {
            typedef Loki::SingletonHolder<PluginManager> PluginManager;
        };

        class PluginManager_i
        {
            public:
            //typedef boost::shared_ptr<pcx::DynamicLibrary> DynamicLibraryPtr;
			typedef Loki::SmartPtr<pcx::DynamicLibrary> DynamicLibraryPtr;
            
			DynamicLibraryPtr NULL_LIB_PTR;
			
			typedef std::map<std::string, DynamicLibraryPtr > DynamicLibraryMap;
            typedef std::vector<PF_ExitFunc> ExitFuncVec;
            typedef std::vector<PF_RegisterParams> RegistrationVec;
            typedef std::map<std::string, PF_RegisterParams> RegistrationMap;

            public:
                PF_PlatformServices d_PlatformServices;

                DynamicLibraryMap d_DynamicLibMap;
                ExitFuncVec d_ExitFuncVec;

                RegistrationMap d_ExactMatchMap;
                RegistrationVec d_WildCardVec;

                PluginManager_i()
                {
                    d_PlatformServices.version.major=1;
                    d_PlatformServices.version.minor=0;
                    d_PlatformServices.invokeService = 0; // init in loadAll
                    d_PlatformServices.registerObject = PluginManager::registerObject;
                }

                ~PluginManager_i()
                {
					d_DynamicLibMap.clear();
                    d_ExactMatchMap.clear();
                    d_WildCardVec.clear();
                    d_ExitFuncVec.clear();
                }

               DynamicLibraryPtr loadLibrary(const std::string& path, 
                        std::string& err)
                {
                    pcx::DynamicLibrary *d = pcx::DynamicLibrary::load(path.c_str(), err);
                    if(!d)
                        return NULL_LIB_PTR;
					
					DynamicLibraryPtr ptr(d);
                    d_DynamicLibMap[path]=ptr;
                    return ptr;
                }
        };

        PluginManager::PluginManager()
            :d_this(new PluginManager_i)
        {}

        PluginManager::~PluginManager()
        {
            shutdown();
            delete d_this;
        }

        PluginManager& PluginManager::getInstance()
        {
            return PluginManagerHolder::PluginManager::Instance();
        }

        int PluginManager::loadAll(const char* path,
                PF_InvokeServiceFunc func)
        {
            if(!path || strlen(path)==0)
            {
                return -1;
            }

            if(!bf::exists(path) || !bf::is_directory(path))
                return -1;

            d_this->d_PlatformServices.invokeService = func;

            for(pcx::PluginIter it(path); it; ++it)
            {
                load(it()); 
            }

            return 0;
        }

        int PluginManager::initializePlugin(PF_InitFunc initFunc)
        {
            PluginManager& pm = getInstance();

            PF_ExitFunc exitFunc = initFunc(&pm.d_this->d_PlatformServices);
            if(!exitFunc)
            {
                return -1;
            }

            pm.d_this->d_ExitFuncVec.push_back(exitFunc);

            return 0;
        }


        int PluginManager::load(const char* path)
        {
            bf::path dl_path = path;
            if(bf::is_symlink(dl_path)) 
            {
                dl_path = bf::read_symlink(dl_path);
                if(bf::is_empty(dl_path))
                    return -1;
            }

            //Dont load the same library 
            if(d_this->d_DynamicLibMap.find(dl_path.string())!=d_this->d_DynamicLibMap.end())
            {
              return -1; 
            }

            std::string err;
            PluginManager_i::DynamicLibraryPtr d = d_this->loadLibrary(dl_path.c_str(), err);
            if(!d)
                return -1;

            PF_InitFunc initFunc = (PF_InitFunc)(d->getSymbol("PF_initPlugin"));
            if(!initFunc)
                return -1;

            int res = initializePlugin(initFunc);
            if(res<0)
                return res;

            return 0;
        }


        void* PluginManager::createObject(const char* type, IObjectAdapter &adapter)
        {
            if(!type || !(*type))
            {
                return NULL;
            }

            //* is not an valid object type
            if(strcmp(type, "*")==0)
            {
                return NULL;
            }

            PF_ObjectParams np;
            np.objectType = type;
            np.platformServices = &d_this->d_PlatformServices;

            if(d_this->d_ExactMatchMap.find(type)!=d_this->d_ExactMatchMap.end())
            {
                PF_RegisterParams & rp = d_this->d_ExactMatchMap[type];
                void* object = rp.createFunc(&np);
                if(object)
                {
                    if(rp.programmingLanguage == PF_ProgrammingLanguage_C)
                        object = adapter.adapt(object, rp.destroyFunc);
                    return object;
                }
            }

            for(size_t i=0; i<d_this->d_WildCardVec.size(); i++)
            {
                PF_RegisterParams &rp = d_this->d_WildCardVec[i];
                void* object = rp.createFunc(&np);
                if(object)
                {
                    if(rp.programmingLanguage==PF_ProgrammingLanguage_C)
                    {
                        object = adapter.adapt(object, rp.destroyFunc);
                    }

                    //promote registration to d_ExactMatchMap
                    int res = registerObject(np.objectType, &rp);
                    if(res<0)
                    {
                        //Serious Framework should report or log it
                        rp.destroyFunc(object);
                        return NULL;
                    }
                    return object;
                }
            }
            //No result
            return NULL;
        }

        int PluginManager::shutdown()
        {
            int result = 0;
            for(PluginManager_i::ExitFuncVec::iterator func = 
                    d_this->d_ExitFuncVec.begin(); 
                    func!=d_this->d_ExitFuncVec.end(); ++func)
            {
                try{
                    result = (*func)();
                }
                catch(...)
                {
                    result = -1;
                }
            }

            return result;
        }


        static bool isValid(const char* objectType, const PF_RegisterParams *params)
        {
            if(!objectType || !(*objectType))
            {
                return false;
            }
            if(!params || !params->createFunc || !params->destroyFunc)
                return false;

            return true;
        }

        int PluginManager::registerObject(const char* type,
                const PF_RegisterParams *params)
        {
            if(!isValid(type, params))
            {
                return -1;
            }

            PluginManager &pm = getInstance();

            
            //Verify that versions match
            PF_PluginAPI_Version v = pm.d_this->d_PlatformServices.version;
            if(v.major !=params->version.major)
            {
                return -1;
            }

            std::string key(type);
            if(key=="*")
            {
                pm.d_this->d_WildCardVec.push_back(*params);
                return 0;
            }

            if(pm.d_this->d_ExactMatchMap.find(key)!=pm.d_this->d_ExactMatchMap.end())
            {
                return -1;
            }

            pm.d_this->d_ExactMatchMap[key] = *params;
			return 0;
        }


        PF_PlatformServices& PluginManager::getPlatformServices()
        {
            return d_this->d_PlatformServices;
        }
    }
}
