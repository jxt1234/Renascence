#ifndef UTILS_TIMES_H
#define UTILS_TIMES_H

class GP_Clock
{
    public:
        GP_Clock(int func);
        int reset();
        virtual ~GP_Clock();
    protected:
        int mStart;
        int mId;
};



#endif
