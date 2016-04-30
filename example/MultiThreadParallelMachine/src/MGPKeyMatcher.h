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
#ifndef MGPKEYMATCHER_H
#define MGPKEYMATCHER_H
#include "lowlevelAPI/IParallelMachine.h"
#include <map>
#include <vector>
class MGPKeyMatcher:public GPRefCount
{
public:
    class Key:public GPRefCount
    {
    public:
        Key(unsigned int* keys, unsigned int keyNumber);
        virtual ~Key();

        unsigned int* getKey() const {return mKeys;}
        unsigned int getKeyNumber() const {return mKeyNumber;}
        bool match(Key* other);
    private:
        unsigned int* mKeys;
        unsigned int mKeyNumber;
    };

    MGPKeyMatcher(GPPieces** inputs, unsigned int inputNumber, GPPieces* output, const std::vector<std::pair<unsigned int, unsigned int>>& outputKey, IGPFloatFunction* condition);
    virtual ~MGPKeyMatcher();
    
    const std::map<GPPtr<Key>, std::vector<GPPtr<Key>>>& get() const {return mKeyMatches;}
    
private:
    std::map<GPPtr<Key>, std::vector<GPPtr<Key>>> mKeyMatches;
};
#endif
