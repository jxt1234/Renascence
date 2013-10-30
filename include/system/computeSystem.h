#ifndef GP_COMPUTE_SYSTEM_H
#define GP_COMPUTE_SYSTEM_H
#include "core/function.h"
#include "core/status.h"
#include "xml/xmlFunctionLoader.h"

class computeSystem
{
    public:
        inline const std::vector<computeFunction>& getMapFunction() {return mFunctions;}
        computeFunction getFunction(int id) {if (id < mFunctions.size() && 0<=id) return mFunctions[id]; return NULL;}
        void loadFuncXml(xmlFunctionLoader& loader, void* &handle);
        void clear();
        std::vector<int> getAllFunctionIDs();
        //Basic Api
        int allocateStatus(int id);
        inline const std::vector<int>& getInputFunctions(){return mInputId;}
        const std::vector<std::vector<int> >& getAvailableFunctionInputs(int functionId);
        const std::vector<int>& getOutputFunctions(){return mOutputId;}
        void print();
        computeSystem();
        virtual ~computeSystem();
    protected:
        std::vector<int> loadStatus(const std::vector<xmlFunctionLoader::status>& sta, void* handle);
        void constructBasicTable();
    private:
        void _loadUnit(xmlFunctionLoader func);
        std::vector<std::vector<std::vector<int> > > mBasicTable;
        std::vector<computeFunction> mFunctions;
        std::vector<std::vector<int> > mStatusTable;
        std::vector<int> mOutputId;//可做为根结点的函数Id
        std::vector<int> mInputId;//作为叶结点的函数Id
};

#endif
