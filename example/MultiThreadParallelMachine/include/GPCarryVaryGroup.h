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
#ifndef MATH_GPCARRYVARYGROUP_H
#define MATH_GPCARRYVARYGROUP_H
class GPCarryVaryGroup
{
public:
    GPCarryVaryGroup(unsigned int* dimesions, unsigned int number);
    ~GPCarryVaryGroup();
    bool next(unsigned int* enums, unsigned int number);
    void start(unsigned int* enums, unsigned int number);
private:
    unsigned int mNumber;
    unsigned int* mDimesions;
};
#endif
