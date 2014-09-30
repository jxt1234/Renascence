#include "producer/GPGraphicADF.h"
typedef GPGraphicADF::Point _POINT;

_POINT::Point(const GPFunctionDataBase::function* f):mFunc(f)
{
}

_POINT::~Point()
{
}

void _POINT::_clean()
{
    for (INPUT_ITER i = mInputs.begin(); i!=mInputs.end(); i++)
    {
        i->second = NULL;//The Unit will be unreference
    }
}

void _POINT::_reset()
{
    _clean();
    for (OUTPUT_ITER i=mOutputs.begin(); i!=mOutputs.end(); i++)
    {
        (*i)->_reset();
    }
}

GP_Output _POINT::collect()
{
    GP_Output result;
    for (INPUT_ITER i = mInputs.begin(); i!=mInputs.end(); i++)
    {
        Unit* u = (i->second).get();
        result.output.push_back(u->warp());
    }
    _clean();
    return result;
}

bool _POINT::_flow(bool clean)
{
    if (NULL != mFunc)
    {
        GP_Input inputs;
        for (INPUT_ITER i = mInputs.begin(); i!=mInputs.end(); i++)
        {
            void* c = i->second->content();
            assert(NULL!=c);
            inputs.push_back(c);
        }
        GP_Output out = mFunc->basic(inputs);
        assert(out.size() == mOutputs.size());
        for (int i=0; i<out.size(); ++i)
        {
            GPPtr<Unit> u = new Unit(out.output[i]);
            (mOutputs.at(i))->receive(u, this);
        }
        if (clean)
        {
            _clean();
        }
    }
    for (OUTPUT_ITER i = mOutputs.begin(); i!=mOutputs.end(); i++)
    {
        if ((*i)->ready())
        {
            (*i)->_flow(clean);
        }
    }
    return true;
}
bool _POINT::ready() const
{
    bool result = true;
    for (INPUT_ITER_CONST i = mInputs.begin(); i!=mInputs.end(); i++)
    {
        if (NULL == (i->second).get())
        {
            result = false;
            break;
        }
    }
    return result;
}

void _POINT::receive(GPPtr<GPGraphicADF::Unit> u, _POINT* source)
{
    /*Output points shouldn't be called by this function*/
    /*For input points can only has one output, it just pass the data to midpoints*/
    if (NULL == mFunc)
    {
        assert(mInputs.empty());
        assert(mOutputs.size() == 1);
        (*(mOutputs.begin()))->receive(u, this);
    }
    /*Mid points should has position for the data, otherwise the connection is wrong*/
    bool find = false;
    for (INPUT_ITER i=mInputs.begin(); i!=mInputs.end(); i++)
    {
        if (i->first == source)
        {
            i->second = u;
            find = true;
            break;
        }
    }
    assert(true == find);
}
