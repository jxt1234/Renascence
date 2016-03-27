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
struct GPPieces
{
    unsigned int pKeySize[10];
    unsigned int nKeyNumber;

    void* pMeta;
    void (*pFree)(void* pMeta);
    
    /*This Function will create a new GPContent, and the caller should destroy it self*/
    GPContents*(*pLoad)(void* pMeta, unsigned int* pKey, unsigned int keynum);

    /*This Function will both destroy or own the memory of GPContent, after calling this function, should not use GPContent*/
    void(*pSave)(void* pMeta, unsigned int* pKey, unsigned int keynum, GPContents* c);
    
    
    GPContents* load(unsigned int* pKey, unsigned int keynum)
    {
        return pLoad(pMeta, pKey, keynum);
    }
    
    void save(unsigned int* pKey, unsigned int keynum, GPContents* c)
    {
        pSave(pMeta, pKey, keynum, c);
    }
    
    void clear()
    {
        pFree(pMeta);
    }
};
#endif
