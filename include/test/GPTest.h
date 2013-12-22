#ifndef TEST_GLTEST_H
#define TEST_GLTEST_H

#include <vector>
#include <string>

class GPTest
{
    friend class GPTestSuit;
    public:
        virtual void run()=0;
        GPTest(){}
        virtual ~GPTest(){}
    private:
        std::string name;
};

class GPTestSuit
{
    public:
        static void run();
        static GPTestSuit* get();
        void insert(GPTest* t, const char* name);
        //
        GPTestSuit(){}
        ~GPTestSuit();
    private:
        static GPTestSuit* global;
        std::vector<GPTest*> mTests;
};

template <class T>
class GPTestRegister
{
    public:
        GPTestRegister(const char* claim)
        {
            T* test = new T;
            GPTestSuit* ts = GPTestSuit::get();
            ts->insert(test, claim);
        }
        ~GPTestRegister(){}
};
#endif
