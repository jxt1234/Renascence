#ifndef GP_COMPUTE_SYSTEM_H
#define GP_COMPUTE_SYSTEM_H
#include "core/function.h"
#include "core/functionDataBase.h"
#include "user/func_status_user.h"
#include "xml/xmlFunctionLoader.h"

class computeSystem
{
    public:
        mapFunction getMapFunction();
        computeFunction map(int functionId);
        void loadTable(const char* file=NULL);
        void loadFuncXml(xmlFunctionLoader& loader);
        vector<int> getAllFunctionIDs();
        //Basic Api
        inline const vector<int>& getInputFunctions(){return mInputId;}
        inline const vector<vector<int> >& getAvailableFunctionInputs(int functionId){return mBasicTable[functionId];}
        inline const vector<int>& getOutputFunctions(){return mOutputId;}
        //Option API, for spection function set
        const vector<int>& getAvailablemFunctions(int functionId);
        inline pair<int, int> getInputNumbers(int functionId){return mInputNumbers[functionId];}
        void print();
        computeSystem();
        virtual ~computeSystem();
    protected:
        void constructBasicTable();
    private:
        void clear();
        vector<vector<vector<int> > > mBasicTable;
        vector<computeFunction> mFunctions;
        vector<vector<int> > mTable;
        vector<int> mOutputId;//可做为根结点的函数Id
        vector<int> mInputId;//作为叶结点的函数Id
        vector<pair<int, int> > mInputNumbers;//各函数所需的最小及最大输入数
};

#endif
