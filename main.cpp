/******************************************************************
   Copyright 2013, Jiang Xiao-tang

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
#include "api/evolution.h"
#include "xml/xmlGPLoader.h"

#include <iostream>
using namespace std;

void print(GP_Output& out)
{
    if (out.output.size()>0)
    {
        double* result = (double*)(out.output[0].content);
        cout <<"result = "<<(*result)<<endl;
    }
}


int main()
{
    GP_evolution(10, 1000, "func.xml", NULL, "result.xml");
    GP_RunXml("result.xml", NULL, print);
    return 1;
}
