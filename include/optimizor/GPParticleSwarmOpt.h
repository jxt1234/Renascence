/******************************************************************
   Copyright 2014, Jiang Xiao-tang

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
#ifndef OPTIMIZOR_GPPARTICLESWARMOPT_H
#define OPTIMIZOR_GPPARTICLESWARMOPT_H
#include "IGPOptimizor.h"
#include <vector>
class GPParticleSwarmOpt:public IGPOptimizor
{
    public:
        GPParticleSwarmOpt(PFLOAT Vmax, int groupsize = 20, int time = 1000, PFLOAT dis=0.0001, PFLOAT theta1=2.1, PFLOAT theta2=2.1);
        virtual ~GPParticleSwarmOpt();
        virtual GPPtr<GPParameter> vFindBest(int mN, IGPOptimizor::OPTFUNC computer, PFLOAT* target) const;
    private:
        class Particle:public RefCount
        {
            public:
                Particle(int n, PFLOAT limit, PFLOAT the1, PFLOAT the2);
                virtual ~Particle();
                inline PFLOAT fit() const {return mFit;}
                inline void setFit(PFLOAT fit) {mFit = fit;}
                void randomInit();
                void fix();
                PFLOAT distance(GPPtr<Particle> mP);
                GPPtr<GPParameter> expand() const;
                void updateFit(PFLOAT fit);
                void move(GPPtr<Particle> best);
            protected:
                PFLOAT* mX;
                PFLOAT* mP;
                PFLOAT* mV;
                int mN;
                PFLOAT mFit;
                /*Para*/
                PFLOAT the1;
                PFLOAT the2;
                PFLOAT Max;
        };
        PFLOAT max_distance(const std::vector<GPPtr<Particle> >& group) const;
        PFLOAT mVmax;
        PFLOAT mThe1;
        PFLOAT mThe2;
        int mSize;
        int mTimes;
        PFLOAT mMinDistance;
};
#endif
