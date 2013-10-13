#ifndef USER_PACKAGE_H
#define USER_PACKAGE_H

/*We can not define a destructor function and use freeCallBack for GP_Output, because the freeCallBack is called by the system outside*/
struct GP_Output
{
    struct GP_Unit
    {
        void* content;
        void(*freeCallBack)(void*);
    };
    std::vector<GP_Unit> output;
};

#define GP_OUTPUT_SINGLE_EXIT(x, result) GP_Output x;x.output.push_back(result);return x;

#endif
