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
#include <fstream>
#include "utils/AutoClean.h"
#include <assert.h>
using namespace std;

int main(int argc, char** argv)
{
    /*Prepare Runtime*/
    GPProducer* sys = GPProducerFactory::create();
    AutoClean<GPProducer> __clean_sys(sys);
    ifstream is;
    assert(FilePath::open(FilePath::RUNTIME, is));
    sys->vAddContent(is, NULL);
    is.close();
    /*Load Standard function*/
    assert(FilePath::open(FilePath::STANDARD, is));
    IGPAutoDefFunction* function = sys->vCreateFunctionFromIS(is);
    AutoClean<IGPAutoDefFunction> __clean_funtion(function);
    /*Find all input and output file*/

    return 1;
}
