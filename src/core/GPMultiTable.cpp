#include "core/GPMultiTable.h"
#include "utils/debug.h"
void* GPMultiTable::vGetFunction(const std::string& name)
{
    for (auto t:mTables)
    {
        void* f = t->vGetFunction(name);
        if (NULL!=f)
        {
            return f;
        }
    }
    return NULL;
}
GPMultiTable::GPMultiTable()
{
    
}
GPMultiTable::~GPMultiTable()
{
    GPASSERT(mTables.size() == mNeedDelete.size());
    int n = (int)(mTables.size());
    for (int i=0; i<n; ++i)
    {
        if (mNeedDelete[i])
        {
            delete mTables[i];
        }
    }
}

void GPMultiTable::addTable(IFunctionTable* table, bool need_delete)
{
    GPASSERT(NULL!=table);
    mTables.push_back(table);
    mNeedDelete.push_back(need_delete);
}
