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

#include "core/GPStreamUtils.h"
#include "utils/GPDebug.h"
GPPtr<GPBlock> GPStreamUtils::read(GPStream* input, bool merge)
{
    GPASSERT(NULL!=input);
    GPASSERT(input->isValid());
    static const size_t size = 2 << 12;
    GPPtr<GPBlock> head = new GPBlock(size);
    GPPtr<GPBlock> currentnode = head;
    while(1)
    {
        auto c = currentnode->contents();
        auto real_size = input->read(c, size);
        currentnode->resize(real_size);
        if (input->isEnd())
        {
            break;
        }
        GPPtr<GPBlock> newnode = new GPBlock(size);
        currentnode->setNext(newnode);
        currentnode = newnode;
    }
    if (merge)
    {
        head = GPBlock::merge(head.get());
    }
    return head;
}
void GPStreamUtils::write(GPWStream* output, GPPtr<GPBlock> content)
{
    GPASSERT(NULL!=output);
    GPASSERT(output->isValid());
    GPASSERT(content.get()!=NULL);
    const GPBlock* cur = content.get();
    while (NULL!=cur)
    {
        output->write(cur->contents(), cur->size());
        cur = cur->getNext();
    }
}
