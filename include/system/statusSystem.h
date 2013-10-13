#ifndef STATUS_SYSTEM_H
#define STATUS_SYSTEM_H
#include "core/GeneticProgram.h"
#include "core/status.h"
#include "user/func_status_user.h"
#include <vector>
#include "xml/xmlFunctionLoader.h"
vector<void*> basicStatusMap(int statusId);
class statusSystem
{
    public:
        statusSystem(){status_init();}
        ~statusSystem();
        void loadFuncXml(xmlFunctionLoader& loader);
        //Serverice start
        void loadTable(const char* file = "db/status.table");
        void init();
        //Serverice support API
        void randomlyChange(int statusId);
        //respond for request
        int allocateStatus(int functionId);
        vector<int> allocateStatus(const vector<int>& functionIds, bool repeat=true);//If the repeat is false, it will use the same statusID for one functionId
        bool varyStatus(int statusId);
        statusMapFunction getMapFunction(){return basicStatusMap;}
    private:
        vector<vector<int> > mStatusType;
};
#endif
