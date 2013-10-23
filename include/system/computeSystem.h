#ifndef GP_COMPUTE_SYSTEM_H
#define GP_COMPUTE_SYSTEM_H
#include "core/function.h"
#include "core/status.h"
#include "xml/xmlFunctionLoader.h"

class computeSystem
{
    public:
        inline const std::vector<computeFunction>& getMapFunction() {return mFunctions;}
        void loadFuncXml(xmlFunctionLoader& loader, void* &handle);
        void clear();
        std::vector<int> getAllFunctionIDs();
        //Basic Api
        int allocateStatus(int id);
        inline const std::vector<int>& getInputFunctions(){return mInputId;}
        inline const std::vector<std::vector<int> >& getAvailableFunctionInputs(int functionId){return mBasicTable[functionId];}
        inline const std::vector<int>& getOutputFunctions(){return mOutputId;}
        //Option API, for spection function set
        const std::vector<int>& getAvailablemFunctions(int functionId);
        inline std::pair<int, int> getInputNumbers(int functionId){return mInputNumbers[functionId];}
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
        std::vector<std::pair<int, int> > mInputNumbers;//各函数所需的最小及最大输入数
};

#endif
