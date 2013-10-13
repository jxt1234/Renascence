#include "core/GeneticProgram.h"
#include "userFunction.h"
#include "system/computeSystem.h"
#include "system/statusSystem.h"
#include "system/GenerateSystem.h"
#include "core/evolutionTreeV2.h"
#include <time.h>
#include "core/group.h"

void test()
{
    srand((unsigned) time(NULL));
    int p[] ={1,0,2,0,0,2,2,0,0,2, 0,0,2,0,0};
    vector<int> style(p, p+sizeof(p)/sizeof(int));
    GeneticProgram GP(style);
    {
        BasicSet* output = (BasicSet*)GP.compute(basicMap);
        printf("size = %d, sizeofdouble=%d \n", output->size, sizeof(double));
        double* data = (double*)output->content;
        printf("data = %f\n", *data);
        delete output;
    }

    computeSystem* compute = new computeSystem;
    compute->loadTable();
    compute->print();

    statusSystem status;
    status.loadTable();

    GenerateSystem gen(compute, &status);
    style = gen.getRandSequence();
    vector<int> style2 = gen.searchOneSequence();
    for (int i=0; i<style.size(); ++i)
    {
        printf("%d ",style[i]);
    }
    printf("\n style2.size = %d\n", style2.size());
    for (int i=0; i<style2.size(); ++i)
    {
        printf("%d ",style2[i]);
    }
    printf("\n");
    GeneticProgram GP2(style);
    {
        BasicSet* output = (BasicSet*)GP2.compute(basicMap);
        printf("size = %d, sizeofdouble=%d \n", output->size, sizeof(double));
        double* data = (double*)output->content;
        printf("data = %f\n", *data);
        delete output;
    }

    GP2=GP;
    {
        BasicSet* output = (BasicSet*)GP2.compute(basicMap);
        printf("size = %d, sizeofdouble=%d \n", output->size, sizeof(double));
        double* data = (double*)output->content;
        printf("data = %f\n", *data);
        delete output;
    }
}

#include "xml/xmlReader.h"
#include "xml/xmlFunctionLoader.h"
#include "system/xmlGenerateSystem.h"

void testXml1()
{
    xmlReaderTest test;
    test.loadFile("func.xml");
}
void testXml2()
{
    xmlFunctionLoader test2;
    test2.loadFile("func.xml");
    test2.print();
}

void testEvolution()
{
    xmlGenerateSystem gen("func.xml");
    evolutionTreeV2::setGenSystem(&gen);
    evolution_group<evolutionTreeV2> group(200);
    group.evolution(10);
    printf("%f, \n",(group.get_best())->get_fit());
}

int main()
{
    srand((unsigned) time(NULL));
    //test();
    testXml2();
    testXmlGen();
    //testEvolution();
    return 1;
}
