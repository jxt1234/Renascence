#include "MGPThreadPool.h"
#include "MGPUtils.h"

int main()
{
    MGPThreadPool* pool = new MGPThreadPool(std::vector<void*>{NULL, NULL});
    static int i = 0;
    class Run : public MGPThreadPool::Runnable
    {
    public:
        virtual void vRun(void*)
        {
            for (int q=0; q<100; ++q)
            {
                FUNC_PRINT(i);
                i++;
            }
        }
    };
    
    MGPSema* sema1 = pool->pushTask(new Run);
    MGPSema* sema2 = pool->pushTask(new Run);
    MGPSema* sema3 = pool->pushTask(new Run);
    MGPSema* sema4 = pool->pushTask(new Run);
    sema2->wait();
    sema1->wait();
    sema3->wait();
    sema4->wait();
    delete sema1;
    delete sema2;
    delete sema3;
    delete sema4;
    
    delete pool;
    return 1;
}
