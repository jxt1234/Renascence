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
#include "math/GPCarryVaryGroup.h"
#include "utils/AutoStorage.h"
#include "utils/GPClock.h"


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



MGPKeyMatcher::MGPKeyMatcher(IGPKeyIterator* iterator)
{
    //GPCLOCK;
    MGPASSERT(NULL!=iterator);
    auto size = iterator->vGetSize();
    int sumDim = size.first;
    int outputKeyStorage = size.second;
    
    AUTOSTORAGE(keyOutput, unsigned int, outputKeyStorage);
    AUTOSTORAGE(keyCurrent, unsigned int, sumDim);
    bool hasData = iterator->vRewind(keyCurrent, keyOutput);
    if (!hasData)
    {
        MGPASSERT(0);
        return;
    }
    do
    {
        /*Record the key*/
        GPPtr<Key> inputKey = new Key(keyCurrent, sumDim);
        GPPtr<Key> outputKey = new Key(keyOutput, outputKeyStorage);
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
            mKeyMatches.insert(std::make_pair(outputKey.get(), std::vector<GPPtr<Key>>{inputKey}));
            mOutputKeys.push_back(outputKey);
        }
    } while (iterator->vNext(keyCurrent, keyOutput));
}

MGPKeyMatcher::~MGPKeyMatcher()
{
    mKeyMatches.clear();
}
