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
#include "core/GPProducerFactory.h"
#include "core/GPProducer.h"
#include <fstream>
#include "utils/AutoClean.h"
#include <assert.h>
#include "xml/xmlGPDataLoader.h"
using namespace std;

int main(int argc, char** argv)
{
    /*Prepare Runtime*/
    assert(argc>=2);
    FilePath::setEnvPath(argv[1]);
    GPProducer* sys = GPProducerFactory::create();
    AutoClean<GPProducer> __clean_sys(sys);
    ifstream is;
    bool res = FilePath::open(FilePath::RUNTIME, is);
    assert(true == res);
    sys->vAddContent(is, NULL);
    is.close();
    /*Load Standard function*/
    res = FilePath::open(FilePath::STANDARD, is);
    assert(true == res);
    IGPAutoDefFunction* function = sys->vCreateFunctionFromIS(is);
    AutoClean<IGPAutoDefFunction> __clean_funtion(function);
    is.close();
    /*Find all input and output file*/
    vector<string> inputFiles;
    vector<string> outputFiles;
    res = (FilePath::open(FilePath::INPUT, is));
    assert(true == res);
    string temp;
    while (is >> temp)
    {
        inputFiles.push_back(temp);
    }
    is.close();
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
        xmlGPDataLoader lin(*sys);
        xmlGPDataLoader lout(*sys);
        lin.loadFile(inputFiles[i].c_str());
        lout.loadFile(outputFiles[i].c_str());
        GPData* inp = lin.get();
        GPData* out = lout.get();
        GP_Input gp_inp = inp->expand();
        GP_Output gp_out = function->run(gp_inp);
        double res = out->compare(gp_out);
        result_os <<res <<"\n";
        gp_out.clear();
    }
    result_os.close();

    return 1;
}
