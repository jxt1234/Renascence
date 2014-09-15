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
#ifndef MATH_CARRYGROUP2_H
#define MATH_CARRYGROUP2_H
#include <vector>
#include <assert.h>
template<typename T>
class carryGroup2
{
    public:
        typedef std::vector<T> TEAM;
        void reset()
        {
            mIter.clear();
            mIter.assign(mBase.size(), 0);
            mSize.reserve(mBase.size());
            for (int i=0; i<mBase.size(); ++i)
            {
                TEAM& t = mBase.at(i);
                //TODO Permit it and set invalid
                assert(t.size()>0);
                mSize.push_back(t.size());
                mCurrent.push_back(t.at(0));
            }
        }
        std::vector<std::vector<T> > mBase;
        const std::vector<T>& current() const {return mCurrent;}
        bool next()
        {
            for (int i=0; i<mIter.size(); ++i)
            {
                int cur = mIter.at(i)+1;
                if (cur < mSize.at(i))
                {
                    mIter.at(i) = cur;
                    mCurrent.at(i) = mBase.at(i).at(cur);
                    return true;
                }
                cur = 0;
                mIter.at(i) = cur;
                mCurrent.at(i) = mBase.at(i).at(cur);
            }
            return false;
        }
    private:
        std::vector<int> mSize;
        std::vector<int> mIter;
        TEAM mCurrent;
};
#endif
