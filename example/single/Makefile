include make.self
ALL_INCLUES= include/test/GPTest.h include/math/AbstractPoint.h include/math/carryTree.h include/math/carryArray.h include/math/carryPoint.h include/api/evolution.h include/system/xmlDataBase.h include/system/GenerateSystem.h include/system/recurse_tree.h include/system/computeSystem.h include/system/system_lib.h include/system/xmlGenerateSystem.h include/evolution/evolutionTree.h include/core/status.h include/core/AbstractGP.h include/core/genetic_operator.h include/core/GP_XmlString.h include/core/IDataBase.h include/core/function.h include/core/IGPAutoDefFunction.h include/core/group.h include/core/functionDataBase.h include/core/computeTree.h include/xml/xmlFunctionLoader.h include/xml/xmlGPLoader.h include/xml/xmlReader.h include/utils/statusMachine.h include/utils/BasicTree.h include/utils/debug.h include/utils/GP_Clock.h include/user/IFunctionTable.h include/user/package.h
all: GP_MAIN 
GP_MAIN:  build/main.o
	g++  -O3 -o GP_MAIN  build/main.o  ${SELF_VARIABLES}
build/main.o : ./main.cpp ${ALL_INCLUES}
	g++ -O3 -o build/main.o -c ./main.cpp -Iinclude 

clean:
	rm build/*.o
	rm GP_MAIN