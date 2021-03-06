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
#include "utils/GPRandom.h"
#include <random>
namespace GPRandom
{
    static const int inc = 100000;
    static std::random_device gDevice;
    float rate()
    {
        int r = gDevice()%inc;
        float p = (float)r/(float)inc;
        return p;
    }

    bool init()
    {
        return true;
    }
    int mid(int min_, int max_)
    {
        int r = gDevice()%(max_-min_);
        return r+min_;
    }

    bool reset()
    {
        return true;
    }
};
