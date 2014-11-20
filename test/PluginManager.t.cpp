#include "../plugin.h"
#include "../PluginManager.h"
#include "simple_object.h"
#include "../ObjectAdapter.h"

#include <iostream>
using namespace PointCX::Plugin;

class SimpleObjectAdapter
{
    private:
        SimpleObject* d_obj_p;
        PF_DestroyFunc d_DestroyFunc;

    public:
        SimpleObjectAdapter(SimpleObject* obj, PF_DestroyFunc destroyFunc)
            :d_obj_p(obj), d_DestroyFunc(destroyFunc)
        {
            std::cout << "Creating... SimpleObjectAdapter" << std::endl;
        }

        ~SimpleObjectAdapter()
        {
            std::cout << "Destroying... SimpleObjectAdapter" << std::endl;
            if(d_obj_p && d_DestroyFunc)
            {
                d_DestroyFunc(d_obj_p);
            }
        }

        const char* getName()const
        {
            return d_obj_p->name;
        }

        void setName(const char* name)
        {
			printf("Set Name:%s\n", name);
			if(!d_obj_p)
			{
				return;
			}
			//d_obj_p->name = name;
            strcpy(d_obj_p->name, name);
			printf("Set Name:%s Finished\n", name);
        } 
};

int main(int argc, char** argv)
{
    PluginManager& pm = PluginManager::getInstance();

    int ret = pm.loadAll(argv[1]);

    ObjectAdapter<SimpleObjectAdapter, SimpleObject> adapter;
    void* obj = pm.createObject("SimpleObject", adapter);
	printf("Get Object:%p\n", obj);
    if(obj)
    {
		printf("Cast %p to SimpleObjectAdapter*\n", obj);
        SimpleObjectAdapter* soa = (SimpleObjectAdapter*)obj;
        soa->setName("SimpleObject-00");
        std::cout << "SimpleObjectAdapter.name=" << soa->getName() << std::endl;
        delete soa;
    }else{
		printf("Can not create object with type SimpleObject");
	}
    return ret;
}
