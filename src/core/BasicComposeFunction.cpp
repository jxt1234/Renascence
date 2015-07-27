#include <utils/debug.h>
#include "core/BasicComposeFunction.h"
#include "utils/debug.h"

using namespace std;

//TODO Optimization these

GPBoolADF::GPBoolADF(IGPAutoDefFunction* gp)
{
    GPASSERT(NULL!=gp);
    mGP = gp;
    gp->addRef();
}

GPBoolADF::~GPBoolADF()
{
    mGP->decRef();
}

bool GPBoolADF::value(GPContents* inputs)
{
    GPContents* out = mGP->vRun(inputs);
    bool* res = (bool*)out->get(0);
    bool r = *res;
    out->clear();
    delete out;
    return r;
}



GPCombineADF::GPCombineADF(std::vector<IGPAutoDefFunction*> funcs)
{
    mFunctions = funcs;
    for (int i=0; i<mFunctions.size(); ++i)
    {
        mFunctions[i]->addRef();
        auto inputs = mFunctions[i]->getInputTypes();
        mInputTypes.insert(mInputTypes.end(), inputs.begin(), inputs.end());
        auto outputs = mFunctions[i]->getOutputTypes();
        mOutputTypes.insert(mOutputTypes.end(), outputs.begin(), outputs.end());
    }
}

GPCombineADF::~GPCombineADF()
{
    for (int i=0; i<mFunctions.size(); ++i)
    {
        (mFunctions[i])->decRef();
    }
}

GPContents* GPCombineADF::vRun(GPContents* inputs)
{
    GPContents* res = new GPContents;
    int cur = 0;
    for (int i=0; i<mFunctions.size(); ++i)
    {
        size_t n = (mFunctions[i]->getInputTypes()).size();
        GPASSERT(0<=n);
        GPASSERT(inputs->size() >= cur+n);
        GPContents input;
        for (int j=0; j<n; ++j)
        {
            input.push((*inputs)[cur+j]);
        }
        cur+=n;
        GPContents* out = (mFunctions[i])->vRun(&input);
        for (auto p : out->contents)
        {
            res->push(p);
        }
        delete out;
    }
    return res;
}
int GPCombineADF::vMap(GPPtr<GPParameter> para)
{
    return 0;
}
void GPCombineADF::vSave(std::ostream& os) const
{
    os << "<Combine>\n";
    for (int i=0; i<mFunctions.size(); ++i)
    {
        os << "<Function_"<<i<<">\n";
        (mFunctions[i])->vSave(os);
        os << "</Function_"<<i<<">\n";
    }
    os << "</Combine>\n";
}

IGPAutoDefFunction* GPCombineADF::vCopy() const
{
    return new GPCombineADF(mFunctions);
}

void GPCombineADF::vMutate()
{
    for (auto f:mFunctions)
    {
        f->vMutate();
    }
}


GPSwitchADF::GPSwitchADF(IGPAutoDefFunction* _s, IGPAutoDefFunction* _a, IGPAutoDefFunction* _b)
{
    GPASSERT(NULL!=_s);
    GPASSERT(NULL!=_a);
    GPASSERT(NULL!=_b);

    s = new GPBoolADF(_s);
    a = _a;
    b = _b;

    a->addRef();
    b->addRef();
}

GPSwitchADF::GPSwitchADF(GPBoolADF* _s, IGPAutoDefFunction* _a, IGPAutoDefFunction* _b)
{
    GPASSERT(NULL!=_s);
    GPASSERT(NULL!=_a);
    GPASSERT(NULL!=_b);

    s = _s;
    a = _a;
    b = _b;

    s->addRef();
    a->addRef();
    b->addRef();
    /*TODO: Setup mInputTypes and mOutputTypes*/
}

GPSwitchADF::~GPSwitchADF()
{
    s->decRef();
    a->decRef();
    b->decRef();
}

IGPAutoDefFunction* GPSwitchADF::vCopy() const
{
    IGPAutoDefFunction* r = new GPSwitchADF(s,a,b);
    return r;
}
GPContents* GPSwitchADF::vRun(GPContents* inputs)
{
    bool swi = false;
    size_t cur = 0;
    size_t n = 0;
    IGPAutoDefFunction* tar = NULL;
    //If operator
    {
        n = (s->get()->getInputTypes()).size();
        GPContents inp;
        for (int i=0; i<n; ++i)
        {
            inp.push(inputs->contents[i]);
        }
        swi = s->value(&inp);
    }
    if (swi)
    {
        cur = (s->get()->getInputTypes()).size();
        n = (a->getInputTypes()).size();
        tar = a;
    }
    else
    {
        cur = (s->get()->getInputTypes()).size() + (a->getInputTypes()).size();
        n = (b->getInputTypes()).size();
        tar = b;
    }
    GPContents inp;
    for (int i=0; i<n; ++i)
    {
        inp.push(inputs->contents[i+cur]);
    }
    return tar->vRun(&inp);
}
void GPSwitchADF::vSave(std::ostream& os) const
{
    os << "<Switch>\n";
    os << "<Just>\n";
    s->get()->vSave(os);
    os << "</Just>\n";
    os << "<Option_1>\n";
    a->vSave(os);
    os << "</Option_1>\n";
    os << "<Option_2>\n";
    b->vSave(os);
    os << "</Option_2>\n";
    os << "</Switch>\n";
}

int GPSwitchADF::vMap(GPPtr<GPParameter> para)
{
    return 0;
}

void GPSwitchADF::vMutate()
{
    a->vMutate();
    b->vMutate();
}