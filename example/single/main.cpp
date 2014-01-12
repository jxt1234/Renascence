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

#include "evolution/evolutionTree.h"
#include "core/group.h"
#include "system/xmlGenerateSystem.h"
#include <iostream>
#include <string>
#include <sstream>
#include "xml/xmlGPLoader.h"
#include "utils/debug.h"
using namespace std;

extern const char* Name[];
extern void* f[]; 
extern int gNumber;

class selfFunc:public IFunctionTable
{
	public:
		virtual void* vGetFunction(const std::string& name)
		{
			int n = gNumber;
			void* result = NULL;
			for (int i=0; i<n; ++i)
			{
				if (name == Name[i])
				{
					result = f[i];
					break;
				}
			}
			return result;
		}
};


void randomGener(const char* functionTable, const char* outputXml1, const char* outputXml2)
{
    srand((unsigned) time(NULL));
    xmlGenerateSystem gen;
	selfFunc funcTable;
    gen.addXml(functionTable, &funcTable, false);
    evolutionTree::setGenSystem(&gen);
	evolutionTree tree1;
	evolutionTree tree2;
    string result = gen.xmlPrint(tree1.get());
    ofstream file;
    file.open(outputXml1);
    file<<result;
	result = gen.xmlPrint(tree2.get());
    file.close();
	file.open(outputXml2);
	file << result;
	file.close();
}

void runs(const char* functionXml, const char* input, void(*output)(GP_Output&))
{
    xmlGPLoader load;
    load.loadFile(functionXml);
    GP_Output gp = load.run();
    if (NULL!=output) output(gp);
    GP_Output_clear(gp);
}

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
    randomGener("../../func.xml", "result1.xml", "result2.xml");
}
