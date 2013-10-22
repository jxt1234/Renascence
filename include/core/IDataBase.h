#ifndef CORE_DATABASE_H
#define CORE_DATABASE_H
/*Abstarct Class, used for GP to query status, function name, and relative lib*/
#include <string>
class IDataBase
{
    public:
        virtual void vQueryStatus(int id, std::string& name, std::string& libName) = 0;
        virtual void vQueryFunction(int id, std::string& name, std::string& libName) = 0;

        IDataBase(){}
        virtual ~IDataBase(){}
};

#endif
