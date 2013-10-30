#ifndef GENETIC_PROGRAM_H
#define GENETIC_PROGRAM_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include "function.h"
#include "status.h"
#include <sstream>

#include "core/IDataBase.h"

/*GeneticPoint is a compute point in a GeneticProgram, it collect a seriers inputs from other GeneticPoint, and then compute an result with its own constValue.
  The constValue is mapped from statusMapFunction, which map the statusId to void* constValue
 GeneticPoint is just a symbol, which means it can repress every kind of compute function, so it must work with a const std::vector<computeFunction>&, which can turn its functionId to actual computeFunction*/


struct GeneticPoint
{
    int functionId;
    int statusId;
    std::vector<GeneticPoint*> children;
    GP_Output* save;
    GeneticPoint(int _fun = -1, int _sta = -1, GP_Output* _save=NULL)
    {
        functionId = _fun;
        statusId = _sta;
        save = _save;
    }
    ~GeneticPoint(){}
};

/*A GeneticProgram can be generated by the three ways: Randomly generately, copy from another GeneticProgram and load from files */
class GeneticProgram
{
    public:
        GeneticProgram();
        GeneticProgram(const std::vector<int> &numbers);
        GeneticProgram(const char* file);
        GeneticProgram(const GeneticProgram& program);
        virtual ~GeneticProgram();
        void operator=(const GeneticProgram& program);
        void clear();
        GP_Output compute(IFunctionDataBase* map);
        bool replacePoint(const std::vector<int> &numbers, int n);
        //Save appointed functionIds' output
        void reset();
        void save(IFunctionDataBase* map, const std::vector<int>& functionIds);
        //Output a xml string, which can be write into file directly
        std::string xmlPrint(IDataBase* data);
        //For evolution trees
        std::vector<int> getStatusId();
        std::vector<int> getFuncId();
        //Standard load method
        static void loadUnitFunction(std::vector<int>& result, int functionId, int statusId, int number);
        //Replace pair<old, new> function id
        void replaceFunctionId(std::vector<std::pair<int, int> > pairList);
    protected:
        bool replacePoint(const std::vector<int> &numbers, GeneticPoint* point);
        GP_Output computeUnit(IFunctionDataBase* map, GeneticPoint* point);
        void computeUnitSave(IFunctionDataBase* map, GeneticPoint* point, const std::vector<int> functionIds);
        std::vector<GeneticPoint*> searchAllPoints();
        void deleteUnit(GeneticPoint* point);
        void copyPoints(GeneticPoint* self, GeneticPoint* source);
        std::vector<int> getPointQueue(GeneticPoint* point);
        void xmlPrintUnit(std::ostringstream& res, IDataBase* data, GeneticPoint* point);
    protected:
        GeneticPoint* mRoot;
        std::vector<GeneticPoint*> varyPoints;//Record the points that can be modified
};
#endif
