/******************************************************************
 Copyright 2016, Jiang Xiao-tang
 
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
#ifndef USER_GPPIECES_H
#define USER_GPPIECES_H
#include "GPContents.h"
#include <string>
class GPPieces : public GPRefCount
{
public:
    unsigned int pKeySize[10];
    unsigned int nKeyNumber;
    std::vector<const IStatusType*> pTypes;
    std::string sInfo;

    virtual GPContents* vLoad(unsigned int* pKey, unsigned int keynum) = 0;
    
    virtual long vPreMeasure(unsigned int* pKey, unsigned int keynum) const {return -1;}

    virtual void vSave(unsigned int* pKey, unsigned int keynum, GPContents* c) = 0;
    
    virtual ~GPPieces(){}
protected:
    GPPieces() {}
};
#endif
