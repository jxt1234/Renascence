#include <assert.h>
#include "core/BasicComposeFunction.h"
#include "utils/debug.h"

GPBoolADF::GPBoolADF(IGPAutoDefFunction* gp)
{
    assert(NULL!=gp);
    mGP = gp;
    gp->addRef();
}

GPBoolADF::~GPBoolADF()
{
    mGP->decRef();
}

bool GPBoolADF::value(const GP_Input& inputs)
{
    GP_Output out = mGP->run(inputs);
    bool* res = (bool*)out[0];
    bool r = *res;
    out.clear();
    return r;
}

int GPCombineADF::inputNumber() const
{
    int res=0;
    for (int i=0; i<mFunctions.size(); ++i)
    {
        res+=mFunctions[i]->inputNumber();
    }
    return res;
}

int GPCombineADF::outputNumber() const
{
    int res = 0;
    for (int i=0; i<mFunctions.size(); ++i)
    {
        res+=mFunctions[i]->outputNumber();
    }
    return res;
}

GPCombineADF::GPCombineADF(std::vector<IGPAutoDefFunction*> funcs)
{
    mFunctions = funcs;
    for (int i=0; i<mFunctions.size(); ++i)
    {
        mFunctions[i]->addRef();
    }
}

GPCombineADF::~GPCombineADF()
{
    for (int i=0; i<mFunctions.size(); ++i)
    {
        (mFunctions[i])->decRef();
    }
}

GP_Output GPCombineADF::run(const GP_Input& inputs)
{
    GP_Output res;
    int cur = 0;
    for (int i=0; i<mFunctions.size(); ++i)
    {
        int n = mFunctions[i]->inputNumber();
        //printf("n=%d, i=%d, cur=%d, inputsize=%d\n", n, i, cur, inputs.size());
        assert(0<=n);
        assert(inputs.size() >= cur+n);
        GP_Input input;
        for (int j=0; j<n; ++j)
        {
            input.push_back(inputs[cur+j]);
        }
        cur+=n;
        GP_Output out = (mFunctions[i])->run(input);
        for (int j=0; j<out.output.size(); ++j)
        {
            res.output.push_back(out.output[j]);
        }
    }
    return res;
}
void GPCombineADF::save(std::ostream& os)
{
    os << "<Combine>\n";
    for (int i=0; i<mFunctions.size(); ++i)
    {
        os << "<Function_"<<i<<">\n";
        (mFunctions[i])->save(os);
        os << "</Function_"<<i<<">\n";
    }
    os << "</Combine>\n";
}
void GPCombineADF::load(std::istream& is)
{
}
void GPCombineADF::mutate()
{
    for (int i=0; i<mFunctions.size(); ++i)
    {
        mFunctions[i]->mutate();
    }
}

GPSwitchADF::GPSwitchADF(IGPAutoDefFunction* _s, IGPAutoDefFunction* _a, IGPAutoDefFunction* _b)
{
    assert(NULL!=_s);
    assert(NULL!=_a);
    assert(NULL!=_b);

    s = new GPBoolADF(_s);
    a = _a;
    b = _b;

    a->addRef();
    b->addRef();
}

GPSwitchADF::GPSwitchADF(GPBoolADF* _s, IGPAutoDefFunction* _a, IGPAutoDefFunction* _b)
{
    assert(NULL!=_s);
    assert(NULL!=_a);
    assert(NULL!=_b);

    s = _s;
    a = _a;
    b = _b;

    s->addRef();
    a->addRef();
    b->addRef();
}

GPSwitchADF::~GPSwitchADF()
{
    s->decRef();
    a->decRef();
    b->decRef();
}
GP_Output GPSwitchADF::run(const GP_Input& inputs)
{
    bool swi = false;
    int cur = 0;
    int n = 0;
    IGPAutoDefFunction* tar = NULL;
    //If operator
    {
        n = s->get()->inputNumber();
        GP_Input inp;
        for (int i=0; i<n; ++i)
        {
            inp.push_back(inputs[i]);
        }
        swi = s->value(inp);
    }
    if (swi)
    {
        cur = s->get()->inputNumber();
        n = a->inputNumber();
        tar = a;
    }
    else
    {
        cur = s->get()->inputNumber() + a->inputNumber();
        n = b->inputNumber();
        tar = b;
    }
    GP_Input inp;
    for (int i=0; i<n; ++i)
    {
        inp.push_back(inputs[i+cur]);
    }
    return tar->run(inp);
}
void GPSwitchADF::save(std::ostream& os)
{
    os << "<Switch>\n";
    os << "<Just>\n";
    s->get()->save(os);
    os << "</Just>\n";
    os << "<Option_1>\n";
    a->save(os);
    os << "</Option_1>\n";
    os << "<Option_2>\n";
    b->save(os);
    os << "</Option_2>\n";
    os << "</Switch>\n";
}
void GPSwitchADF::load(std::istream& is)
{
}
void GPSwitchADF::mutate()
{
    s->get()->mutate();
    a->mutate();
    b->mutate();
}
int GPSwitchADF::inputNumber() const
{
    int res = 
        s->get()->inputNumber() + 
        a->inputNumber() +
        b->inputNumber()
        ;
    return res;
}

int GPSwitchADF::outputNumber() const
{
    int res = 
        s->get()->outputNumber() + 
        a->outputNumber() +
        b->outputNumber()
        ;
    return res;
}
