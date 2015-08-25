//
//  GPStreamADFProducer.h
//  GP
//
//  Created by jiangxiaotang on 15/8/25.
//  Copyright (c) 2015年 jiangxiaotang. All rights reserved.
//

#ifndef __GP__GPStreamADFProducer__
#define __GP__GPStreamADFProducer__

#include <stdio.h>
#include "core/GPProducer.h"
class GPStreamADFProducer:public GPProducer
{
public:
    GPStreamADFProducer(const GPFunctionDataBase* sys);
};
#endif /* defined(__GP__GPStreamADFProducer__) */
