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

#ifndef __GP__GPUserStream__
#define __GP__GPUserStream__

#include <stdio.h>
#include "core/GPStreamFactory.h"

class GPUserStream:public GPStreamWrap
{
public:
    GPUserStream(const char* filename);
    virtual ~GPUserStream();
};

class GPUserWStream:public GPWStreamWrap
{
public:
    GPUserWStream(const char* filename);
    virtual ~GPUserWStream();
};
#endif /* defined(__GP__GPUserStream__) */
