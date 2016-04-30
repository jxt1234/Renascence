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
#include "MGPKeyMatcher.h"
#include "MGPUtils.h"
#include <string.h>
#include "GPCarryVaryGroup.h"
#include "AutoStorage.h"


MGPKeyMatcher::Key::Key(unsigned int* keys, unsigned int keyNumber)
{
    MGPASSERT(NULL!=keys);
    MGPASSERT(0<keyNumber);
    mKeyNumber = keyNumber;
    mKeys = new unsigned int[keyNumber];
    ::memcpy(mKeys, keys, keyNumber*sizeof(unsigned int));
}
MGPKeyMatcher::Key::~Key()
{
    delete [] mKeys;
}


bool MGPKeyMatcher::Key::match(Key* other)
{
    MGPASSERT(other->getKeyNumber() == mKeyNumber);
    for (int i=0; i<mKeyNumber; ++i)
    {
        if (other->getKey()[i] != mKeys[i])
        {
            return false;
        }
    }
    return true;
}



MGPKeyMatcher::MGPKeyMatcher(GPPieces** inputs, unsigned int inputNumber, GPPieces* output, const std::vector<std::pair<unsigned int, unsigned int>>& outputKeys, IGPFloatFunction* condition)
{
    MGPASSERT(NULL!=output);
    MGPASSERT(NULL!=inputs);
    MGPASSERT(inputNumber>0);
    /*Compute all dimesions*/
    unsigned int sumDim = 0;
    for (int i=0; i<inputNumber; ++i)
    {
        sumDim += inputs[i]->nKeyNumber;
    }
    AUTOSTORAGE(keyOutput, unsigned int, (int)outputKeys.size());
    AUTOSTORAGE(keyOutputPos, unsigned int, (int)outputKeys.size());
    for (int pos=0; pos < outputKeys.size(); ++pos)
    {
        keyOutputPos[pos] = 0;
        auto p = outputKeys[pos];
        for (int i=0; i<p.first; ++i)
        {
            keyOutputPos[pos] = keyOutputPos[pos] + inputs[i]->nKeyNumber;
        }
    }
    
    AUTOSTORAGE(keyDimesions, unsigned int, sumDim);
    AUTOSTORAGE(keyCurrent, unsigned int, sumDim);
    AUTOSTORAGE(keyCurrentFloat, GPFLOAT, sumDim);
    unsigned int pos = 0;
    for (int i=0; i<inputNumber; ++i)
    {
        ::memcpy(keyDimesions+pos, inputs[i]->pKeySize, sizeof(unsigned int)*inputs[i]->nKeyNumber);
        pos += inputs[i]->nKeyNumber;
    }
    
    GPCarryVaryGroup group(keyDimesions, sumDim);
    group.start(keyCurrent, sumDim);
    do
    {
        if (NULL!=condition)
        {
            for (int i=0; i<sumDim; ++i)
            {
                keyCurrentFloat[i] = keyCurrent[i];
            }
            GPFLOAT c = condition->vRun(keyCurrentFloat, sumDim);
            if (c <= 0)
            {
                continue;
            }
        }
        /*Compute Target Key*/
        for (int i=0; i<outputKeys.size(); ++i)
        {
            keyOutput[i] = keyCurrent[keyOutputPos[i]];
        }
        
        /*Record the key*/
        GPPtr<Key> inputKey = new Key(keyCurrent, sumDim);
        GPPtr<Key> outputKey = new Key(keyOutput, (unsigned int)outputKeys.size());
        bool match = false;
        for (auto& k : mKeyMatches)
        {
            if (k.first -> match(outputKey.get()))
            {
                k.second.push_back(inputKey);
                match = true;
            }
        }
        if (!match)
        {
            mKeyMatches.insert(std::make_pair(outputKey, std::vector<GPPtr<Key>>{inputKey}));
        }
    } while (group.next(keyCurrent, sumDim));
}

MGPKeyMatcher::~MGPKeyMatcher()
{
    mKeyMatches.clear();
}
