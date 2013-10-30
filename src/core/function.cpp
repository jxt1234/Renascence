#include "core/function.h"
void GP_Output_clear(GP_Output& out)
{
    for (int i=0; i<out.output.size(); ++i)
    {
        if (NULL!=out.output[i].freeCallBack)
        {
            out.output[i].freeCallBack(out.output[i].content);
        }
    }
}

