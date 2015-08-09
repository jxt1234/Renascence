/******************************************************************
  Copyright 2014, Jiang Xiao-tang

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
 ******************************************************************/
#include "FilePath.h"
#include "user/GPAPI.h"
#include <fstream>
#include <assert.h>
using namespace std;

int main(int argc, char** argv)
{
    /*Prepare Runtime*/
    assert(argc>=2);
    GP_Set_Stream_Path(argv[1]);
    GPStream* database = FilePath::open(FilePath::RUNTIME);
    AGPProducer* producer = GP_Producer_Create(database, NULL, GP_PRODUCER_TREE);
    FilePath::close(database);
    {
        /*TODO*/
#if 0
        GPStream* functionstream = FilePath::open(FilePath::STANDARD);
        IGPAutoDefFunction* function = GP_Function_Create_ByStream(producer, functionstream);
        FilePath::close(functionstream);
        /*Find all input and output file*/
        vector<string> inputFiles;
        vector<string> outputFiles;
        auto res = FilePath::open(FilePath::INPUT);
        string temp;
        while (is >> temp)
        {
            inputFiles.push_back(temp);
        }
        FilePath::close(res);
        res = (FilePath::open(FilePath::OUTPUT, is));
        assert(true == res);
        while (is >> temp)
        {
            outputFiles.push_back(temp);
        }
        is.close();
        res = (inputFiles.size() == outputFiles.size());
        assert(true == res);
        /*TODO Load Compare function*/
        /*Start to compare*/
        int n = inputFiles.size();
        ofstream result_os(FilePath::file(FilePath::RESULT).c_str());
        for (int i=0; i<n; ++i)
        {
            xmlGPDataLoader lin(*base);
            xmlGPDataLoader lout(*base);
            lin.loadFile(inputFiles[i].c_str());
            lout.loadFile(outputFiles[i].c_str());
            GPData* inp = lin.get();
            GPData* out = lout.get();
            GPContents* gp_inp = inp->expand();
            GPContents* gp_out = function->vRun(gp_inp);
            double res = out->compare(*gp_out);
            result_os <<res <<"\n";
            gp_out->clear();
            delete gp_out;
        }
        result_os.close();
#endif
    }
    GP_Producer_Destroy(producer);

    return 1;
}
