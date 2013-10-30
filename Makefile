ALL_INCLUES= include/math/carryTree.h include/math/carryArray.h include/math/carryPoint.h include/system/xmlDataBase.h include/system/GenerateSystem.h include/system/recurse_tree.h include/system/computeSystem.h include/system/system_lib.h include/system/xmlGenerateSystem.h include/core/status.h include/core/GPRunable.h include/core/IGPRunner.h include/core/evolutionTreeV2.h include/core/genetic_operator.h include/core/GP_XmlString.h include/core/IDataBase.h include/core/function.h include/core/group.h include/core/GeneticProgram.h include/core/functionDataBase.h include/core/computeTree.h include/xml/xmlFunctionLoader.h include/xml/xmlGPLoader.h include/xml/xmlReader.h include/utils/statusMachine.h include/utils/BasicTree.h include/utils/debug.h include/user/func_status_user.h include/user/package.h include/user/total.h
all: GP_MAIN
GP_MAIN:  build/math_carryPoint.o build/math_carryArray.o build/platform_system_lib.o build/core_GeneticProgram.o build/core_GPRunable.o build/core_GP_XmlString.o build/core_status.o build/core_evolutionTreeV2.o build/core_function.o build/xml_xmlFunctionLoader.o build/xml_xmlReader.o build/xml_xmlGPLoader.o build/utils_statusMachine.o build/systems_GenerateSystem.o build/systems_recurse_tree.o build/systems_computeSystem.o build/systems_xmlDataBase.o build/systems_xmlGenerateSystem.o build/main.o build/utils_debug.o
	g++  -O3  -o GP_MAIN  build/math_carryPoint.o build/math_carryArray.o build/platform_system_lib.o build/core_GeneticProgram.o build/core_GPRunable.o build/core_GP_XmlString.o build/core_status.o build/core_evolutionTreeV2.o build/core_function.o build/xml_xmlFunctionLoader.o build/xml_xmlReader.o build/xml_xmlGPLoader.o build/utils_statusMachine.o build/systems_GenerateSystem.o build/systems_recurse_tree.o build/systems_computeSystem.o build/systems_xmlDataBase.o build/systems_xmlGenerateSystem.o build/main.o build/utils_debug.o -ldl 
build/math_carryPoint.o : src/math/carryPoint.cpp ${ALL_INCLUES}
	g++  -O3  -o build/math_carryPoint.o -c src/math/carryPoint.cpp -Iinclude 
build/math_carryArray.o : src/math/carryArray.cpp ${ALL_INCLUES}
	g++  -O3  -o build/math_carryArray.o -c src/math/carryArray.cpp -Iinclude 
build/platform_system_lib.o : src/platform/system_lib.cpp ${ALL_INCLUES}
	g++  -O3  -o build/platform_system_lib.o -c src/platform/system_lib.cpp -Iinclude 
build/core_GeneticProgram.o : src/core/GeneticProgram.cpp ${ALL_INCLUES}
	g++  -O3  -o build/core_GeneticProgram.o -c src/core/GeneticProgram.cpp -Iinclude 
build/core_GPRunable.o : src/core/GPRunable.cpp ${ALL_INCLUES}
	g++  -O3  -o build/core_GPRunable.o -c src/core/GPRunable.cpp -Iinclude 
build/core_GP_XmlString.o : src/core/GP_XmlString.cpp ${ALL_INCLUES}
	g++  -O3  -o build/core_GP_XmlString.o -c src/core/GP_XmlString.cpp -Iinclude 
build/core_status.o : src/core/status.cpp ${ALL_INCLUES}
	g++  -O3  -o build/core_status.o -c src/core/status.cpp -Iinclude 
build/core_evolutionTreeV2.o : src/core/evolutionTreeV2.cpp ${ALL_INCLUES}
	g++  -O3  -o build/core_evolutionTreeV2.o -c src/core/evolutionTreeV2.cpp -Iinclude 
build/core_function.o : src/core/function.cpp ${ALL_INCLUES}
	g++  -O3  -o build/core_function.o -c src/core/function.cpp -Iinclude 
build/xml_xmlFunctionLoader.o : src/xml/xmlFunctionLoader.cpp ${ALL_INCLUES}
	g++  -O3  -o build/xml_xmlFunctionLoader.o -c src/xml/xmlFunctionLoader.cpp -Iinclude 
build/xml_xmlReader.o : src/xml/xmlReader.cpp ${ALL_INCLUES}
	g++  -O3  -o build/xml_xmlReader.o -c src/xml/xmlReader.cpp -Iinclude 
build/xml_xmlGPLoader.o : src/xml/xmlGPLoader.cpp ${ALL_INCLUES}
	g++  -O3  -o build/xml_xmlGPLoader.o -c src/xml/xmlGPLoader.cpp -Iinclude 
build/utils_statusMachine.o : src/utils/statusMachine.cpp ${ALL_INCLUES}
	g++  -O3  -o build/utils_statusMachine.o -c src/utils/statusMachine.cpp -Iinclude 
build/systems_GenerateSystem.o : src/systems/GenerateSystem.cpp ${ALL_INCLUES}
	g++  -O3  -o build/systems_GenerateSystem.o -c src/systems/GenerateSystem.cpp -Iinclude 
build/systems_recurse_tree.o : src/systems/recurse_tree.cpp ${ALL_INCLUES}
	g++  -O3  -o build/systems_recurse_tree.o -c src/systems/recurse_tree.cpp -Iinclude 
build/systems_computeSystem.o : src/systems/computeSystem.cpp ${ALL_INCLUES}
	g++  -O3  -o build/systems_computeSystem.o -c src/systems/computeSystem.cpp -Iinclude 
build/systems_xmlDataBase.o : src/systems/xmlDataBase.cpp ${ALL_INCLUES}
	g++  -O3  -o build/systems_xmlDataBase.o -c src/systems/xmlDataBase.cpp -Iinclude 
build/systems_xmlGenerateSystem.o : src/systems/xmlGenerateSystem.cpp ${ALL_INCLUES}
	g++  -O3  -o build/systems_xmlGenerateSystem.o -c src/systems/xmlGenerateSystem.cpp -Iinclude 
build/main.o : ./main.cpp ${ALL_INCLUES}
	g++  -O3  -o build/main.o -c ./main.cpp -Iinclude 
build/utils_debug.o : src/utils/debug.c ${ALL_INCLUES}
	gcc  -O3  -o build/utils_debug.o -c src/utils/debug.c -Iinclude 

clean:
	rm build/*.o
	rm GP_MAIN