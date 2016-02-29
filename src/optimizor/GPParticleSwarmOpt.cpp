#include "optimizor/GPParticleSwarmOpt.h"
#include "utils/GPRandom.h"
#include <string.h>

GPParticleSwarmOpt::Particle::Particle(int _n, GPFLOAT limit, GPFLOAT _the1, GPFLOAT _the2)
{
    Max = limit;
    the1 = _the1;
    the2 = _the2;
    mN = _n;
    mX = new GPFLOAT[mN];
    mP = new GPFLOAT[mN];
    mV = new GPFLOAT[mN];
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
    GPFLOAT* _p = mP->attach();
    for (int i=0; i<mN; ++i)
    {
        _p[i] = (mX[i]+Max)/2.0/Max;
    }
    return mP;
}
GPFLOAT GPParticleSwarmOpt::Particle::distance(GPPtr<Particle> mP)
{
    GPASSERT(mN == mP->mN);
    GPFLOAT* x1 = mX;
    GPFLOAT* x2 = mP->mX;
    GPFLOAT dis = 0;
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
    ::memcpy(mP, mX, mN*sizeof(GPFLOAT));
}
void GPParticleSwarmOpt::Particle::fix()
{
    /*Invalidate mX as mP for output*/
    ::memcpy(mX, mP, mN*sizeof(GPFLOAT));
}
void GPParticleSwarmOpt::Particle::updateFit(GPFLOAT fit)
{
    if (mFit < fit)
    {
        mFit = fit;
        ::memcpy(mP, mX, mN*sizeof(GPFLOAT));
    }
}
void GPParticleSwarmOpt::Particle::move(GPPtr<Particle> best)
{
    GPASSERT(mN == best->mN);
    GPFLOAT* x = mX;
    GPFLOAT* p = mP;
    GPFLOAT* v = mV;
    GPFLOAT* pg = best->mP;
    for (int i=0; i<mN; ++i)
    {
        GPFLOAT u1 = GPRandom::rate()*the1;
        GPFLOAT u2 = GPRandom::rate()*the2;
        v[i] = (v[i] + u1*(p[i]-x[i]) + u2*(pg[i]-x[i]))*0.718;
        x[i] = v[i] + x[i];

        if (x[i] > Max) x[i] = Max;
        if (x[i] < -Max) x[i] = -Max;
    }
}

GPParticleSwarmOpt::GPParticleSwarmOpt(GPFLOAT Vmax, int groupsize, int time, GPFLOAT dis, GPFLOAT theta1, GPFLOAT theta2)
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

GPFLOAT GPParticleSwarmOpt::max_distance(const std::vector<GPPtr<Particle> >& group) const
{
    GPFLOAT _max_distance = 0.0;
    for (int i=0; i<group.size(); ++i)
    {
        for (int j=i+1; j<group.size(); ++j)
        {
            GPFLOAT dis = group[i]->distance(group[j]);
            if (dis > _max_distance)
            {
                _max_distance = dis;
            }
        }
    }
    return _max_distance;
}
GPPtr<GPParameter> GPParticleSwarmOpt::vFindBest(int n, IGPOptimizor::OPTFUNC computer, GPFLOAT* target) const
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
        GPFLOAT fit = computer(p->expand());
        p->setFit(fit);
        group.push_back(p);
    }
    GPPtr<Particle> bestP;
    for (int i=0; i<mTimes; ++i)
    {
        for (int j=0; j<mSize; ++j)
        {
            GPPtr<Particle> p = group[j];
            GPFLOAT fit = computer(p->expand());
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
        GPFLOAT maxDis_before = max_distance(group);
        /*Move all Particle*/
        for (int j=0; j<mSize; ++j)
        {
            GPPtr<Particle> p = group[j];
            p->move(bestP);
        }
        /*Get the max distance after, Determin if we can break*/
        GPFLOAT maxDis_after = max_distance(group);
        if (maxDis_before < mMinDistance*n && maxDis_after < mMinDistance*n)
        {
            break;
        }
    }
    bestP->fix();
    return bestP->expand();
}
