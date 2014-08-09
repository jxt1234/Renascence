all:libGP.so test.out gpsuit.out 

ALL_INCLUESlibGP.so= include/math/AbstractPoint.h include/math/carryTree.h include/math/carryArray.h include/math/carryPoint.h include/math/GPSingleTree.h include/system/xmlDataBase.h include/system/GenerateSystem.h include/system/mutateTree.h include/system/recurse_tree.h include/system/system_lib.h include/system/xmlGenerateSystem.h include/evolution/GPEvolutionGroup.h include/evolution/AbstractEvolutionGroup.h include/core/status.h include/core/GPFunctionDataBase.h include/core/AbstractGP.h include/core/GPProducerFactory.h include/core/funcStatusType.h include/core/genetic_operator.h include/core/GP_XmlString.h include/core/IDataBase.h include/core/BasicComposeFunction.h include/core/function.h include/core/GPData.h include/core/IGPAutoDefFunction.h include/core/group.h include/core/GPProducer.h include/core/computeTree.h include/xml/xmlGPDataLoader.h include/xml/xmlFunctionLoader.h include/xml/xmlGPLoader.h include/xml/xmlReader.h include/xml/xmlTree.h include/utils/statusMachine.h include/utils/RefCount.h include/utils/AutoClean.h include/utils/BasicTree.h include/utils/debug.h include/utils/GP_Clock.h include/head.h include/user/IFunctionTable.h include/user/package.h

libGP.so:  build/src_math_GPSingleTree.o build/src_math_AbstractPoint.o build/src_math_carryPoint.o build/src_math_carryArray.o build/src_system_GenerateSystem.o build/src_system_recurse_tree.o build/src_system_system_lib.o build/src_system_mutateTree.o build/src_system_xmlDataBase.o build/src_system_xmlGenerateSystem.o build/src_platform_system_lib.o build/src_evolution_GPEvolutionGroup.o build/src_core_BasicComposeFunction.o build/src_core_GPData.o build/src_core_GPProducerFactory.o build/src_core_GP_XmlString.o build/src_core_status.o build/src_core_AbstractGP.o build/src_core_function.o build/src_core_funcStatusType.o build/src_core_GPFunctionDataBase.o build/src_xml_xmlFunctionLoader.o build/src_xml_xmlReader.o build/src_xml_xmlGPDataLoader.o build/src_xml_xmlGPLoader.o build/src_xml_xmlTree.o build/src_utils_GP_Clock.o build/src_utils_statusMachine.o build/src_utils_debug.o 
	g++  -O3 -fPIC --share  -o libGP.so  build/src_math_GPSingleTree.o build/src_math_AbstractPoint.o build/src_math_carryPoint.o build/src_math_carryArray.o build/src_system_GenerateSystem.o build/src_system_recurse_tree.o build/src_system_system_lib.o build/src_system_mutateTree.o build/src_system_xmlDataBase.o build/src_system_xmlGenerateSystem.o build/src_platform_system_lib.o build/src_evolution_GPEvolutionGroup.o build/src_core_BasicComposeFunction.o build/src_core_GPData.o build/src_core_GPProducerFactory.o build/src_core_GP_XmlString.o build/src_core_status.o build/src_core_AbstractGP.o build/src_core_function.o build/src_core_funcStatusType.o build/src_core_GPFunctionDataBase.o build/src_xml_xmlFunctionLoader.o build/src_xml_xmlReader.o build/src_xml_xmlGPDataLoader.o build/src_xml_xmlGPLoader.o build/src_xml_xmlTree.o build/src_utils_GP_Clock.o build/src_utils_statusMachine.o build/src_utils_debug.o  -lpthread -ldl ${SELF_VARIABLES}
build/src_math_GPSingleTree.o : src/math/GPSingleTree.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_math_GPSingleTree.o -c src/math/GPSingleTree.cpp -Iinclude 
build/src_math_AbstractPoint.o : src/math/AbstractPoint.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_math_AbstractPoint.o -c src/math/AbstractPoint.cpp -Iinclude 
build/src_math_carryPoint.o : src/math/carryPoint.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_math_carryPoint.o -c src/math/carryPoint.cpp -Iinclude 
build/src_math_carryArray.o : src/math/carryArray.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_math_carryArray.o -c src/math/carryArray.cpp -Iinclude 
build/src_system_GenerateSystem.o : src/system/GenerateSystem.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_system_GenerateSystem.o -c src/system/GenerateSystem.cpp -Iinclude 
build/src_system_recurse_tree.o : src/system/recurse_tree.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_system_recurse_tree.o -c src/system/recurse_tree.cpp -Iinclude 
build/src_system_system_lib.o : src/system/system_lib.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_system_system_lib.o -c src/system/system_lib.cpp -Iinclude 
build/src_system_mutateTree.o : src/system/mutateTree.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_system_mutateTree.o -c src/system/mutateTree.cpp -Iinclude 
build/src_system_xmlDataBase.o : src/system/xmlDataBase.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_system_xmlDataBase.o -c src/system/xmlDataBase.cpp -Iinclude 
build/src_system_xmlGenerateSystem.o : src/system/xmlGenerateSystem.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_system_xmlGenerateSystem.o -c src/system/xmlGenerateSystem.cpp -Iinclude 
build/src_platform_system_lib.o : src/platform/system_lib.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_platform_system_lib.o -c src/platform/system_lib.cpp -Iinclude 
build/src_evolution_GPEvolutionGroup.o : src/evolution/GPEvolutionGroup.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_evolution_GPEvolutionGroup.o -c src/evolution/GPEvolutionGroup.cpp -Iinclude 
build/src_core_BasicComposeFunction.o : src/core/BasicComposeFunction.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_core_BasicComposeFunction.o -c src/core/BasicComposeFunction.cpp -Iinclude 
build/src_core_GPData.o : src/core/GPData.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_core_GPData.o -c src/core/GPData.cpp -Iinclude 
build/src_core_GPProducerFactory.o : src/core/GPProducerFactory.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_core_GPProducerFactory.o -c src/core/GPProducerFactory.cpp -Iinclude 
build/src_core_GP_XmlString.o : src/core/GP_XmlString.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_core_GP_XmlString.o -c src/core/GP_XmlString.cpp -Iinclude 
build/src_core_status.o : src/core/status.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_core_status.o -c src/core/status.cpp -Iinclude 
build/src_core_AbstractGP.o : src/core/AbstractGP.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_core_AbstractGP.o -c src/core/AbstractGP.cpp -Iinclude 
build/src_core_function.o : src/core/function.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_core_function.o -c src/core/function.cpp -Iinclude 
build/src_core_funcStatusType.o : src/core/funcStatusType.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_core_funcStatusType.o -c src/core/funcStatusType.cpp -Iinclude 
build/src_core_GPFunctionDataBase.o : src/core/GPFunctionDataBase.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_core_GPFunctionDataBase.o -c src/core/GPFunctionDataBase.cpp -Iinclude 
build/src_xml_xmlFunctionLoader.o : src/xml/xmlFunctionLoader.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_xml_xmlFunctionLoader.o -c src/xml/xmlFunctionLoader.cpp -Iinclude 
build/src_xml_xmlReader.o : src/xml/xmlReader.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_xml_xmlReader.o -c src/xml/xmlReader.cpp -Iinclude 
build/src_xml_xmlGPDataLoader.o : src/xml/xmlGPDataLoader.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_xml_xmlGPDataLoader.o -c src/xml/xmlGPDataLoader.cpp -Iinclude 
build/src_xml_xmlGPLoader.o : src/xml/xmlGPLoader.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_xml_xmlGPLoader.o -c src/xml/xmlGPLoader.cpp -Iinclude 
build/src_xml_xmlTree.o : src/xml/xmlTree.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_xml_xmlTree.o -c src/xml/xmlTree.cpp -Iinclude 
build/src_utils_GP_Clock.o : src/utils/GP_Clock.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_utils_GP_Clock.o -c src/utils/GP_Clock.cpp -Iinclude 
build/src_utils_statusMachine.o : src/utils/statusMachine.cpp   ${ALL_INCLUESlibGP.so}
	g++ -O3 -fPIC -o build/src_utils_statusMachine.o -c src/utils/statusMachine.cpp -Iinclude 
build/src_utils_debug.o : src/utils/debug.c   ${ALL_INCLUESlibGP.so}
	gcc -O3 -fPIC -o build/src_utils_debug.o -c src/utils/debug.c -Iinclude 

ALL_INCLUEStest.out= include/math/AbstractPoint.h include/math/carryTree.h include/math/carryArray.h include/math/carryPoint.h include/math/GPSingleTree.h include/system/xmlDataBase.h include/system/GenerateSystem.h include/system/mutateTree.h include/system/recurse_tree.h include/system/system_lib.h include/system/xmlGenerateSystem.h include/evolution/GPEvolutionGroup.h include/evolution/AbstractEvolutionGroup.h include/core/status.h include/core/GPFunctionDataBase.h include/core/AbstractGP.h include/core/GPProducerFactory.h include/core/funcStatusType.h include/core/genetic_operator.h include/core/GP_XmlString.h include/core/IDataBase.h include/core/BasicComposeFunction.h include/core/function.h include/core/GPData.h include/core/IGPAutoDefFunction.h include/core/group.h include/core/GPProducer.h include/core/computeTree.h include/xml/xmlGPDataLoader.h include/xml/xmlFunctionLoader.h include/xml/xmlGPLoader.h include/xml/xmlReader.h include/xml/xmlTree.h include/utils/statusMachine.h include/utils/RefCount.h include/utils/AutoClean.h include/utils/BasicTree.h include/utils/debug.h include/utils/GP_Clock.h include/head.h include/user/IFunctionTable.h include/user/package.h

test.out:  build/test_main.o build/test_GPcombineTest.o build/test_AbstractPointTest.o build/test_GPXmlEvolutionTest.o build/test_GPSingleTreeTest.o build/test_GPloadXmlTreeTest.o build/test_GPGPDataTest.o build/test_GPTest.o build/test_GPxmlTreeTest.o build/test_GPSearchIOTest.o libGP.so
	g++  -O3 -fPIC -o test.out  build/test_main.o build/test_GPcombineTest.o build/test_AbstractPointTest.o build/test_GPXmlEvolutionTest.o build/test_GPSingleTreeTest.o build/test_GPloadXmlTreeTest.o build/test_GPGPDataTest.o build/test_GPTest.o build/test_GPxmlTreeTest.o build/test_GPSearchIOTest.o  ./libGP.so -ldl -lpthread  ${SELF_VARIABLES}
build/test_main.o : test/main.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -O3 -fPIC -o build/test_main.o -c test/main.cpp -Iinclude 
build/test_GPcombineTest.o : test/GPcombineTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -O3 -fPIC -o build/test_GPcombineTest.o -c test/GPcombineTest.cpp -Iinclude 
build/test_AbstractPointTest.o : test/AbstractPointTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -O3 -fPIC -o build/test_AbstractPointTest.o -c test/AbstractPointTest.cpp -Iinclude 
build/test_GPXmlEvolutionTest.o : test/GPXmlEvolutionTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -O3 -fPIC -o build/test_GPXmlEvolutionTest.o -c test/GPXmlEvolutionTest.cpp -Iinclude 
build/test_GPSingleTreeTest.o : test/GPSingleTreeTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -O3 -fPIC -o build/test_GPSingleTreeTest.o -c test/GPSingleTreeTest.cpp -Iinclude 
build/test_GPloadXmlTreeTest.o : test/GPloadXmlTreeTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -O3 -fPIC -o build/test_GPloadXmlTreeTest.o -c test/GPloadXmlTreeTest.cpp -Iinclude 
build/test_GPGPDataTest.o : test/GPGPDataTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -O3 -fPIC -o build/test_GPGPDataTest.o -c test/GPGPDataTest.cpp -Iinclude 
build/test_GPTest.o : test/GPTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -O3 -fPIC -o build/test_GPTest.o -c test/GPTest.cpp -Iinclude 
build/test_GPxmlTreeTest.o : test/GPxmlTreeTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -O3 -fPIC -o build/test_GPxmlTreeTest.o -c test/GPxmlTreeTest.cpp -Iinclude 
build/test_GPSearchIOTest.o : test/GPSearchIOTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -O3 -fPIC -o build/test_GPSearchIOTest.o -c test/GPSearchIOTest.cpp -Iinclude 

ALL_INCLUESgpsuit.out= include/math/AbstractPoint.h include/math/carryTree.h include/math/carryArray.h include/math/carryPoint.h include/math/GPSingleTree.h include/system/xmlDataBase.h include/system/GenerateSystem.h include/system/mutateTree.h include/system/recurse_tree.h include/system/system_lib.h include/system/xmlGenerateSystem.h include/evolution/GPEvolutionGroup.h include/evolution/AbstractEvolutionGroup.h include/core/status.h include/core/GPFunctionDataBase.h include/core/AbstractGP.h include/core/GPProducerFactory.h include/core/funcStatusType.h include/core/genetic_operator.h include/core/GP_XmlString.h include/core/IDataBase.h include/core/BasicComposeFunction.h include/core/function.h include/core/GPData.h include/core/IGPAutoDefFunction.h include/core/group.h include/core/GPProducer.h include/core/computeTree.h include/xml/xmlGPDataLoader.h include/xml/xmlFunctionLoader.h include/xml/xmlGPLoader.h include/xml/xmlReader.h include/xml/xmlTree.h include/utils/statusMachine.h include/utils/RefCount.h include/utils/AutoClean.h include/utils/BasicTree.h include/utils/debug.h include/utils/GP_Clock.h include/head.h include/user/IFunctionTable.h include/user/package.h

gpsuit.out:  build/GPSuit_main.o build/GPSuit_FilePath.o libGP.so
	g++  -O3 -fPIC -o gpsuit.out  build/GPSuit_main.o build/GPSuit_FilePath.o  ./libGP.so ${SELF_VARIABLES}
build/GPSuit_main.o : GPSuit/main.cpp libGP.so  ${ALL_INCLUESgpsuit.out}
	g++ -O3 -fPIC -o build/GPSuit_main.o -c GPSuit/main.cpp -Iinclude 
build/GPSuit_FilePath.o : GPSuit/FilePath.cpp libGP.so  ${ALL_INCLUESgpsuit.out}
	g++ -O3 -fPIC -o build/GPSuit_FilePath.o -c GPSuit/FilePath.cpp -Iinclude 

clean:
	rm build/*.o
	rm libGP.so
	rm test.out
	rm gpsuit.out