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
        GPParticleSwarmOpt(GPFLOAT Vmax, int groupsize = 20, int time = 1000, GPFLOAT dis=0.0001, GPFLOAT theta1=2.1, GPFLOAT theta2=2.1);
        virtual ~GPParticleSwarmOpt();
        virtual GPPtr<GPParameter> vFindBest(int mN, IGPOptimizor::OPTFUNC computer, GPFLOAT* target) const;
    private:
        class Particle:public RefCount
        {
            public:
                Particle(int n, GPFLOAT limit, GPFLOAT the1, GPFLOAT the2);
                virtual ~Particle();
                inline GPFLOAT fit() const {return mFit;}
                inline void setFit(GPFLOAT fit) {mFit = fit;}
                void randomInit();
                void fix();
                GPFLOAT distance(GPPtr<Particle> mP);
                GPPtr<GPParameter> expand() const;
                void updateFit(GPFLOAT fit);
                void move(GPPtr<Particle> best);
            protected:
                GPFLOAT* mX;
                GPFLOAT* mP;
                GPFLOAT* mV;
                int mN;
                GPFLOAT mFit;
                /*Para*/
                GPFLOAT the1;
                GPFLOAT the2;
                GPFLOAT Max;
        };
        GPFLOAT max_distance(const std::vector<GPPtr<Particle> >& group) const;
        GPFLOAT mVmax;
        GPFLOAT mThe1;
        GPFLOAT mThe2;
        int mSize;
        int mTimes;
        GPFLOAT mMinDistance;
};
#endif
