#include "test/GPTest.h"
#include "utils/GPDebug.h"
#include <stdlib.h>

GPTestSuit* GPTestSuit::global = NULL;

GPTestSuit* GPTestSuit::get()
{
    if (global ==NULL) global = new GPTestSuit;
    return global;
}

GPTestSuit::~GPTestSuit()
{
    for (int i=0; i<mTests.size(); ++i)
    {
        delete mTests[i];
    }
    mTests.clear();
}

void GPTestSuit::insert(GPTest* t, const char* name)
{
    t->name = name;
    mTests.push_back(t);
}

void GPTestSuit::run()
{
    GPTestSuit::get();
    for (int i=0; i < (global->mTests).size();++i)
    {
        GPTest* t = (global->mTests[i]);
        FUNC_PRINT_ALL((t->name).c_str(), s);
        int start = clock();
        t->run();
        int end = clock();
        FUNC_PRINT((end-start));
    }
}
