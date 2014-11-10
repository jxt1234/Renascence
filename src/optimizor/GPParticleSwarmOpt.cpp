#include "optimizor/GPParticleSwarmOpt.h"
#include "utils/GPRandom.h"
#include <string.h>

GPParticleSwarmOpt::Particle::Particle(int _n, PFLOAT limit, PFLOAT _the1, PFLOAT _the2)
{
    Max = limit;
    the1 = _the1;
    the2 = _the2;
    mN = _n;
    mX = new PFLOAT[mN];
    mP = new PFLOAT[mN];
    mV = new PFLOAT[mN];
}

GPParticleSwarmOpt::Particle::~Particle()
{
    delete [] mX;
    delete [] mP;
    delete [] mV;
}
GPPtr<GPParameter> GPParticleSwarmOpt::Particle::expand() const
{
    GPPtr<GPParameter> mP = new GPParameter(mN);
    PFLOAT* _p = mP->attach();
    for (int i=0; i<mN; ++i)
    {
        _p[i] = (mX[i]+Max)/2.0/Max;
    }
    return mP;
}
PFLOAT GPParticleSwarmOpt::Particle::distance(GPPtr<Particle> mP)
{
    GPASSERT(mN == mP->mN);
    PFLOAT* x1 = mX;
    PFLOAT* x2 = mP->mX;
    PFLOAT dis = 0;
    for (int i=0; i<mN; ++i)
    {
        dis+= ((x1[i]-x2[i])*(x1[i]-x2[i]));
    }
    return dis;
}
void GPParticleSwarmOpt::Particle::randomInit()
{
    for (int i=0; i<mN; ++i)
    {
        mX[i] = Max*(GPRandom::rate()*2.0-1.0);
        mV[i] = Max*(GPRandom::rate()*2.0-1.0);
    }
    ::memcpy(mP, mX, mN*sizeof(PFLOAT));
}
void GPParticleSwarmOpt::Particle::fix()
{
    /*Invalidate mX as mP for output*/
    ::memcpy(mX, mP, mN*sizeof(PFLOAT));
}
void GPParticleSwarmOpt::Particle::updateFit(PFLOAT fit)
{
    if (mFit < fit)
    {
        mFit = fit;
        ::memcpy(mP, mX, mN*sizeof(PFLOAT));
    }
}
void GPParticleSwarmOpt::Particle::move(GPPtr<Particle> best)
{
    GPASSERT(mN == best->mN);
    PFLOAT* x = mX;
    PFLOAT* p = mP;
    PFLOAT* v = mV;
    PFLOAT* pg = best->mP;
    for (int i=0; i<mN; ++i)
    {
        PFLOAT u1 = GPRandom::rate()*the1;
        PFLOAT u2 = GPRandom::rate()*the2;
        v[i] = (v[i] + u1*(p[i]-x[i]) + u2*(pg[i]-x[i]))*0.718;
        x[i] = v[i] + x[i];

        if (x[i] > Max) x[i] = Max;
        if (x[i] < -Max) x[i] = -Max;
    }
}

GPParticleSwarmOpt::GPParticleSwarmOpt(PFLOAT Vmax, int groupsize, int time, PFLOAT dis, PFLOAT theta1, PFLOAT theta2)
{
    mVmax = Vmax;
    mThe1 = theta1;
    mThe2 = theta2;
    mSize = groupsize;
    mTimes = time;
    mMinDistance = dis*dis;
}
GPParticleSwarmOpt::~GPParticleSwarmOpt()
{
}

PFLOAT GPParticleSwarmOpt::max_distance(const std::vector<GPPtr<Particle> >& group) const
{
    PFLOAT _max_distance = 0.0;
    for (int i=0; i<group.size(); ++i)
    {
        for (int j=i+1; j<group.size(); ++j)
        {
            PFLOAT dis = group[i]->distance(group[j]);
            if (dis > _max_distance)
            {
                _max_distance = dis;
            }
        }
    }
    return _max_distance;
}
GPPtr<GPParameter> GPParticleSwarmOpt::vFindBest(int n, GPPtr<IComputer> computer, PFLOAT* target) const
{
    GPASSERT(n>0);
    GPASSERT(mSize>=1);
    GPASSERT(mTimes>=1);
    std::vector<GPPtr<Particle> > group;
    group.reserve(mSize);

    GPRandom::init();
    for (int i=0; i<mSize; ++i)
    {
        GPPtr<Particle> p = (new Particle(n, mVmax, mThe1, mThe2));
        p->randomInit();
        PFLOAT fit = computer->run(p->expand());
        p->setFit(fit);
        group.push_back(p);
    }
    GPPtr<Particle> bestP;
    for (int i=0; i<mTimes; ++i)
    {
        for (int j=0; j<mSize; ++j)
        {
            GPPtr<Particle> p = group[j];
            PFLOAT fit = computer->run(p->expand());
            p->updateFit(fit);
        }
        /*Find Best One*/
        bestP = group[0];
        for (int j=1; j<mSize; ++j)
        {
            if (group[j]->fit() > bestP->fit())
            {
                bestP = group[j];
            }
        }
        if (NULL!=target)
        {
            if (*target <= bestP->fit())
            {
                break;
            }
        }
        /*Get the max distance before, Determin if we can break*/
        PFLOAT maxDis_before = max_distance(group);
        /*Move all Particle*/
        for (int j=0; j<mSize; ++j)
        {
            GPPtr<Particle> p = group[j];
            p->move(bestP);
        }
        /*Get the max distance after, Determin if we can break*/
        PFLOAT maxDis_after = max_distance(group);
        if (maxDis_before < mMinDistance*n && maxDis_after < mMinDistance*n)
        {
            break;
        }
    }
    bestP->fix();
    return bestP->expand();
}
