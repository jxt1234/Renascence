#include "MGPThreadPool.h"
#include "MGPUtils.h"

int main()
{
    MGPThreadPool* pool = new MGPThreadPool(4);
    static int i = 0;
    class Run : public MGPThreadPool::Runnable
    {
    public:
        virtual void vRun()
        {
            for (int q=0; q<100; ++q)
            {
                FUNC_PRINT(i);
                i++;
            }
        }
    };
    
    auto sema1 = pool->pushTask(new Run);
    auto sema2 = pool->pushTask(new Run);
    sema2->wait();
    sema1->wait();
    delete sema1;
    delete sema2;
    
    
    delete pool;
    return 1;
}
