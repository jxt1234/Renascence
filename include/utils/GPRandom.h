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
#ifndef UTILS_GPRANDOM_H
#define UTILS_GPRANDOM_H
namespace GPRandom
{
    //Return rand float number [0.0~1.0)
    float rate();
    //Return rand int [min_, max_)
    int mid(int min_, int max_);
    //init random seed
    bool init();
    bool reset();
};
#endif
