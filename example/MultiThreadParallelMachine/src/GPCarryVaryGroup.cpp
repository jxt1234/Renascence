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
#include "GPCarryVaryGroup.h"
#include <string.h>
#include "MGPUtils.h"
GPCarryVaryGroup::GPCarryVaryGroup(unsigned int* dimesions, unsigned int number)
{
    MGPASSERT(NULL!=dimesions);
    MGPASSERT(number>0);
    mDimesions = new unsigned int[number];
    ::memcpy(mDimesions, dimesions, sizeof(unsigned int)*number);
    mNumber = number;
}
GPCarryVaryGroup::~GPCarryVaryGroup()
{
    delete [] mDimesions;
}
bool GPCarryVaryGroup::next(unsigned int* enums, unsigned int number)
{
    MGPASSERT(number == mNumber);
    enums[0]++;
    for (int i=0; i<number-1; ++i)
    {
        if (enums[i] < mDimesions[i])
        {
            return true;
        }
        enums[i] = 0;
        enums[i+1]++;
    }
    return enums[number-1] < mDimesions[number-1];
}
void GPCarryVaryGroup::start(unsigned int* enums, unsigned int number)
{
    ::memset(enums, 0, number*sizeof(unsigned int));
}
