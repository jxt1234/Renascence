#ifndef ALGORITHM_ABSTRACTPOINT_H
#define ALGORITHM_ABSTRACTPOINT_H

#include <vector>

class AbstractPoint
{
    public:
        /*This class will alloc a new AbstractPoint and copy the contents from src*/
        class IPointCopy
        {
            public:
                virtual AbstractPoint* copy(AbstractPoint* src) = 0;
        };
        AbstractPoint(){}
        /*Use for different types of tree to transform*/
        static AbstractPoint* deepCopy(AbstractPoint* src, IPointCopy* copy);
        virtual ~AbstractPoint();
    protected:
        void addPoint(AbstractPoint* p);
        std::vector<AbstractPoint*> mChildren;
};


#endif
