/******************************************************************
   Copyright 2017, Jiang Xiao-tang

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
#include "GPFrontEnd.h"
#include "pystring.h"
#include "utils/GPDebug.h"

static GP__Point* _create(const std::string& sentence)
{
    return NULL;
}

GP__PointGroup* GPFrontEnd::vCreate(const char* content, const char** errorInfo) const
{
    GPASSERT(NULL!=content);
    std::string program = content;
    std::vector<std::string> sentences;
    pystring::partition(program, ";", sentences);
    std::vector<GP__Point*> points;
    
    std::vector<std::string> errors;
    auto errorFunction = [&errors](const char* error){
        
    };
    
    for (auto& sentence : sentences)
    {
        auto p = _create(sentence);
        if (NULL!=p)
        {
            points.push_back(p);
        }
    }
    
    
    
    return NULL;
}
