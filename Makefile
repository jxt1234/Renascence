all:libGP.a libGP.so test.out gpsuit.out example.out gptool.out 

ALL_INCLUESlibGP.a= include//core/BasicComposeFunction.h include//core/genetic_operator.h include//core/GP_XmlString.h include//core/GPComputePoint.h include//core/GPData.h include//core/GPFactory.h include//core/GPFunctionDataBase.h include//core/GPProducer.h include//core/GPStatusContent.h include//core/group.h include//core/IDataBase.h include//core/IGPAutoDefFunction.h include//core/xmlGPDataLoader.h include//evolution/GPEvolutionGroup.h include//head.h include//math/AbstractPoint.h include//math/carryArray.h include//math/carryGroup2.h include//math/carryPoint.h include//math/carryTree.h include//math/FormulaTree.h include//math/GPSingleTree.h include//optimizor/GPGoldenDivideOpt.h include//optimizor/GPNetSearchOpt.h include//optimizor/GPOptimizorFactory.h include//optimizor/GPParticleSwarmOpt.h include//optimizor/IGPOptimizor.h include//producer/GPGraphicADF.h include//producer/GPGraphicProducer.h include//producer/GPTreeADF.h include//producer/GPTreeProducer.h include//system/system_lib.h include//user/adapter.h include//user/GPAPI.h include//user/GPContents.h include//user/IFunctionTable.h include//user/IStatusType.h include//utils/AutoClean.h include//utils/AutoStorage.h include//utils/BasicTree.h include//utils/debug.h include//utils/GP_Clock.h include//utils/GPRandom.h include//utils/RefCount.h include//xml/xmlReader.h include//xml/xmlTree.h

libGP.a:  build/src_core_BasicComposeFunction.o build/src_core_GP_XmlString.o build/src_core_GPComputePoint.o build/src_core_GPData.o build/src_core_GPFactory.o build/src_core_GPFunctionDataBase.o build/src_core_GPStatusContent.o build/src_core_IGPAutoDefFunction.o build/src_core_xmlGPDataLoader.o build/src_evolution_GPEvolutionGroup.o build/src_math_AbstractPoint.o build/src_math_carryArray.o build/src_math_carryPoint.o build/src_math_FormulaTree.o build/src_math_GPSingleTree.o build/src_optimizor_GPGoldenDivideOpt.o build/src_optimizor_GPNetSearchOpt.o build/src_optimizor_GPOptimizorFactory.o build/src_optimizor_GPParticleSwarmOpt.o build/src_optimizor_IGPOptimizor.o build/src_platform_system_lib.o build/src_producer_GPGraphicADF.o build/src_producer_GPGraphicProducer.o build/src_producer_GPSearchTree.o build/src_producer_GPTreeADF.o build/src_producer_GPTreeProducer.o build/src_producer_recurse_tree.o build/src_system_system_lib.o build/src_user_adapter.o build/src_user_GPAPI.o build/src_utils_debug.o build/src_utils_GP_Clock.o build/src_utils_GPRandom.o build/src_xml_xmlReader.o build/src_xml_xmlTree.o 
	ar rcs libGP.a  build/src_core_BasicComposeFunction.o build/src_core_GP_XmlString.o build/src_core_GPComputePoint.o build/src_core_GPData.o build/src_core_GPFactory.o build/src_core_GPFunctionDataBase.o build/src_core_GPStatusContent.o build/src_core_IGPAutoDefFunction.o build/src_core_xmlGPDataLoader.o build/src_evolution_GPEvolutionGroup.o build/src_math_AbstractPoint.o build/src_math_carryArray.o build/src_math_carryPoint.o build/src_math_FormulaTree.o build/src_math_GPSingleTree.o build/src_optimizor_GPGoldenDivideOpt.o build/src_optimizor_GPNetSearchOpt.o build/src_optimizor_GPOptimizorFactory.o build/src_optimizor_GPParticleSwarmOpt.o build/src_optimizor_IGPOptimizor.o build/src_platform_system_lib.o build/src_producer_GPGraphicADF.o build/src_producer_GPGraphicProducer.o build/src_producer_GPSearchTree.o build/src_producer_GPTreeADF.o build/src_producer_GPTreeProducer.o build/src_producer_recurse_tree.o build/src_system_system_lib.o build/src_user_adapter.o build/src_user_GPAPI.o build/src_utils_debug.o build/src_utils_GP_Clock.o build/src_utils_GPRandom.o build/src_xml_xmlReader.o build/src_xml_xmlTree.o ${SELF_VARIABLES}
build/src_core_BasicComposeFunction.o : src/core/BasicComposeFunction.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_core_BasicComposeFunction.o -c src/core/BasicComposeFunction.cpp -Iinclude
  
build/src_core_GP_XmlString.o : src/core/GP_XmlString.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_core_GP_XmlString.o -c src/core/GP_XmlString.cpp -Iinclude
  
build/src_core_GPComputePoint.o : src/core/GPComputePoint.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_core_GPComputePoint.o -c src/core/GPComputePoint.cpp -Iinclude
  
build/src_core_GPData.o : src/core/GPData.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_core_GPData.o -c src/core/GPData.cpp -Iinclude
  
build/src_core_GPFactory.o : src/core/GPFactory.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_core_GPFactory.o -c src/core/GPFactory.cpp -Iinclude
  
build/src_core_GPFunctionDataBase.o : src/core/GPFunctionDataBase.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_core_GPFunctionDataBase.o -c src/core/GPFunctionDataBase.cpp -Iinclude
  
build/src_core_GPStatusContent.o : src/core/GPStatusContent.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_core_GPStatusContent.o -c src/core/GPStatusContent.cpp -Iinclude
  
build/src_core_IGPAutoDefFunction.o : src/core/IGPAutoDefFunction.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_core_IGPAutoDefFunction.o -c src/core/IGPAutoDefFunction.cpp -Iinclude
  
build/src_core_xmlGPDataLoader.o : src/core/xmlGPDataLoader.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_core_xmlGPDataLoader.o -c src/core/xmlGPDataLoader.cpp -Iinclude
  
build/src_evolution_GPEvolutionGroup.o : src/evolution/GPEvolutionGroup.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_evolution_GPEvolutionGroup.o -c src/evolution/GPEvolutionGroup.cpp -Iinclude
  
build/src_math_AbstractPoint.o : src/math/AbstractPoint.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_math_AbstractPoint.o -c src/math/AbstractPoint.cpp -Iinclude
  
build/src_math_carryArray.o : src/math/carryArray.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_math_carryArray.o -c src/math/carryArray.cpp -Iinclude
  
build/src_math_carryPoint.o : src/math/carryPoint.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_math_carryPoint.o -c src/math/carryPoint.cpp -Iinclude
  
build/src_math_FormulaTree.o : src/math/FormulaTree.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_math_FormulaTree.o -c src/math/FormulaTree.cpp -Iinclude
  
build/src_math_GPSingleTree.o : src/math/GPSingleTree.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_math_GPSingleTree.o -c src/math/GPSingleTree.cpp -Iinclude
  
build/src_optimizor_GPGoldenDivideOpt.o : src/optimizor/GPGoldenDivideOpt.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_optimizor_GPGoldenDivideOpt.o -c src/optimizor/GPGoldenDivideOpt.cpp -Iinclude
  
build/src_optimizor_GPNetSearchOpt.o : src/optimizor/GPNetSearchOpt.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_optimizor_GPNetSearchOpt.o -c src/optimizor/GPNetSearchOpt.cpp -Iinclude
  
build/src_optimizor_GPOptimizorFactory.o : src/optimizor/GPOptimizorFactory.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_optimizor_GPOptimizorFactory.o -c src/optimizor/GPOptimizorFactory.cpp -Iinclude
  
build/src_optimizor_GPParticleSwarmOpt.o : src/optimizor/GPParticleSwarmOpt.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_optimizor_GPParticleSwarmOpt.o -c src/optimizor/GPParticleSwarmOpt.cpp -Iinclude
  
build/src_optimizor_IGPOptimizor.o : src/optimizor/IGPOptimizor.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_optimizor_IGPOptimizor.o -c src/optimizor/IGPOptimizor.cpp -Iinclude
  
build/src_platform_system_lib.o : src/platform/system_lib.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_platform_system_lib.o -c src/platform/system_lib.cpp -Iinclude
  
build/src_producer_GPGraphicADF.o : src/producer/GPGraphicADF.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_producer_GPGraphicADF.o -c src/producer/GPGraphicADF.cpp -Iinclude
  
build/src_producer_GPGraphicProducer.o : src/producer/GPGraphicProducer.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_producer_GPGraphicProducer.o -c src/producer/GPGraphicProducer.cpp -Iinclude
  
build/src_producer_GPSearchTree.o : src/producer/GPSearchTree.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_producer_GPSearchTree.o -c src/producer/GPSearchTree.cpp -Iinclude
  
build/src_producer_GPTreeADF.o : src/producer/GPTreeADF.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_producer_GPTreeADF.o -c src/producer/GPTreeADF.cpp -Iinclude
  
build/src_producer_GPTreeProducer.o : src/producer/GPTreeProducer.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_producer_GPTreeProducer.o -c src/producer/GPTreeProducer.cpp -Iinclude
  
build/src_producer_recurse_tree.o : src/producer/recurse_tree.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_producer_recurse_tree.o -c src/producer/recurse_tree.cpp -Iinclude
  
build/src_system_system_lib.o : src/system/system_lib.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_system_system_lib.o -c src/system/system_lib.cpp -Iinclude
  
build/src_user_adapter.o : src/user/adapter.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_user_adapter.o -c src/user/adapter.cpp -Iinclude
  
build/src_user_GPAPI.o : src/user/GPAPI.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_user_GPAPI.o -c src/user/GPAPI.cpp -Iinclude
  
build/src_utils_debug.o : src/utils/debug.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_utils_debug.o -c src/utils/debug.cpp -Iinclude
  
build/src_utils_GP_Clock.o : src/utils/GP_Clock.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_utils_GP_Clock.o -c src/utils/GP_Clock.cpp -Iinclude
  
build/src_utils_GPRandom.o : src/utils/GPRandom.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_utils_GPRandom.o -c src/utils/GPRandom.cpp -Iinclude
  
build/src_xml_xmlReader.o : src/xml/xmlReader.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_xml_xmlReader.o -c src/xml/xmlReader.cpp -Iinclude
  
build/src_xml_xmlTree.o : src/xml/xmlTree.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -fPIC -o build/src_xml_xmlTree.o -c src/xml/xmlTree.cpp -Iinclude
  

ALL_INCLUESlibGP.so= include//core/BasicComposeFunction.h include//core/genetic_operator.h include//core/GP_XmlString.h include//core/GPComputePoint.h include//core/GPData.h include//core/GPFactory.h include//core/GPFunctionDataBase.h include//core/GPProducer.h include//core/GPStatusContent.h include//core/group.h include//core/IDataBase.h include//core/IGPAutoDefFunction.h include//core/xmlGPDataLoader.h include//evolution/GPEvolutionGroup.h include//head.h include//math/AbstractPoint.h include//math/carryArray.h include//math/carryGroup2.h include//math/carryPoint.h include//math/carryTree.h include//math/FormulaTree.h include//math/GPSingleTree.h include//optimizor/GPGoldenDivideOpt.h include//optimizor/GPNetSearchOpt.h include//optimizor/GPOptimizorFactory.h include//optimizor/GPParticleSwarmOpt.h include//optimizor/IGPOptimizor.h include//producer/GPGraphicADF.h include//producer/GPGraphicProducer.h include//producer/GPTreeADF.h include//producer/GPTreeProducer.h include//system/system_lib.h include//user/adapter.h include//user/GPAPI.h include//user/GPContents.h include//user/IFunctionTable.h include//user/IStatusType.h include//utils/AutoClean.h include//utils/AutoStorage.h include//utils/BasicTree.h include//utils/debug.h include//utils/GP_Clock.h include//utils/GPRandom.h include//utils/RefCount.h include//xml/xmlReader.h include//xml/xmlTree.h

libGP.so:  build/src_core_BasicComposeFunction.o build/src_core_GP_XmlString.o build/src_core_GPComputePoint.o build/src_core_GPData.o build/src_core_GPFactory.o build/src_core_GPFunctionDataBase.o build/src_core_GPStatusContent.o build/src_core_IGPAutoDefFunction.o build/src_core_xmlGPDataLoader.o build/src_evolution_GPEvolutionGroup.o build/src_math_AbstractPoint.o build/src_math_carryArray.o build/src_math_carryPoint.o build/src_math_FormulaTree.o build/src_math_GPSingleTree.o build/src_optimizor_GPGoldenDivideOpt.o build/src_optimizor_GPNetSearchOpt.o build/src_optimizor_GPOptimizorFactory.o build/src_optimizor_GPParticleSwarmOpt.o build/src_optimizor_IGPOptimizor.o build/src_platform_system_lib.o build/src_producer_GPGraphicADF.o build/src_producer_GPGraphicProducer.o build/src_producer_GPSearchTree.o build/src_producer_GPTreeADF.o build/src_producer_GPTreeProducer.o build/src_producer_recurse_tree.o build/src_system_system_lib.o build/src_user_adapter.o build/src_user_GPAPI.o build/src_utils_debug.o build/src_utils_GP_Clock.o build/src_utils_GPRandom.o build/src_xml_xmlReader.o build/src_xml_xmlTree.o 
	g++ -std=c++11  -O3 -fPIC  --shared  -lpthread -ldl -o libGP.so  build/src_core_BasicComposeFunction.o build/src_core_GP_XmlString.o build/src_core_GPComputePoint.o build/src_core_GPData.o build/src_core_GPFactory.o build/src_core_GPFunctionDataBase.o build/src_core_GPStatusContent.o build/src_core_IGPAutoDefFunction.o build/src_core_xmlGPDataLoader.o build/src_evolution_GPEvolutionGroup.o build/src_math_AbstractPoint.o build/src_math_carryArray.o build/src_math_carryPoint.o build/src_math_FormulaTree.o build/src_math_GPSingleTree.o build/src_optimizor_GPGoldenDivideOpt.o build/src_optimizor_GPNetSearchOpt.o build/src_optimizor_GPOptimizorFactory.o build/src_optimizor_GPParticleSwarmOpt.o build/src_optimizor_IGPOptimizor.o build/src_platform_system_lib.o build/src_producer_GPGraphicADF.o build/src_producer_GPGraphicProducer.o build/src_producer_GPSearchTree.o build/src_producer_GPTreeADF.o build/src_producer_GPTreeProducer.o build/src_producer_recurse_tree.o build/src_system_system_lib.o build/src_user_adapter.o build/src_user_GPAPI.o build/src_utils_debug.o build/src_utils_GP_Clock.o build/src_utils_GPRandom.o build/src_xml_xmlReader.o build/src_xml_xmlTree.o ${SELF_VARIABLES}

ALL_INCLUEStest.out= include//core/BasicComposeFunction.h include//core/genetic_operator.h include//core/GP_XmlString.h include//core/GPComputePoint.h include//core/GPData.h include//core/GPFactory.h include//core/GPFunctionDataBase.h include//core/GPProducer.h include//core/GPStatusContent.h include//core/group.h include//core/IDataBase.h include//core/IGPAutoDefFunction.h include//core/xmlGPDataLoader.h include//evolution/GPEvolutionGroup.h include//head.h include//math/AbstractPoint.h include//math/carryArray.h include//math/carryGroup2.h include//math/carryPoint.h include//math/carryTree.h include//math/FormulaTree.h include//math/GPSingleTree.h include//optimizor/GPGoldenDivideOpt.h include//optimizor/GPNetSearchOpt.h include//optimizor/GPOptimizorFactory.h include//optimizor/GPParticleSwarmOpt.h include//optimizor/IGPOptimizor.h include//producer/GPGraphicADF.h include//producer/GPGraphicProducer.h include//producer/GPTreeADF.h include//producer/GPTreeProducer.h include//system/system_lib.h include//user/adapter.h include//user/GPAPI.h include//user/GPContents.h include//user/IFunctionTable.h include//user/IStatusType.h include//utils/AutoClean.h include//utils/AutoStorage.h include//utils/BasicTree.h include//utils/debug.h include//utils/GP_Clock.h include//utils/GPRandom.h include//utils/RefCount.h include//xml/xmlReader.h include//xml/xmlTree.h

test.out:  build/test_AbstractPointTest.o build/test_GPAPITest.o build/test_GPcombineTest.o build/test_GPFormulaTest.o build/test_GPGoldenDividerTest.o build/test_GPGPDataTest.o build/test_GPGraphicTest.o build/test_GPloadXmlTreeTest.o build/test_GPNetSearchTest.o build/test_GPPSOTest.o build/test_GPSearchIOTest.o build/test_GPSingleFunctionOptTest.o build/test_GPSingleTreeTest.o build/test_GPTest.o build/test_GPXmlEvolutionTest.o build/test_GPxmlTreeTest.o build/test_main.o libGP.so
	g++ -std=c++11  -O3 -fPIC   ./libGP.so -lpthread -ldl -o test.out  build/test_AbstractPointTest.o build/test_GPAPITest.o build/test_GPcombineTest.o build/test_GPFormulaTest.o build/test_GPGoldenDividerTest.o build/test_GPGPDataTest.o build/test_GPGraphicTest.o build/test_GPloadXmlTreeTest.o build/test_GPNetSearchTest.o build/test_GPPSOTest.o build/test_GPSearchIOTest.o build/test_GPSingleFunctionOptTest.o build/test_GPSingleTreeTest.o build/test_GPTest.o build/test_GPXmlEvolutionTest.o build/test_GPxmlTreeTest.o build/test_main.o ${SELF_VARIABLES}
build/test_AbstractPointTest.o : test/AbstractPointTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -fPIC -o build/test_AbstractPointTest.o -c test/AbstractPointTest.cpp -Iinclude
  
build/test_GPAPITest.o : test/GPAPITest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -fPIC -o build/test_GPAPITest.o -c test/GPAPITest.cpp -Iinclude
  
build/test_GPcombineTest.o : test/GPcombineTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -fPIC -o build/test_GPcombineTest.o -c test/GPcombineTest.cpp -Iinclude
  
build/test_GPFormulaTest.o : test/GPFormulaTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -fPIC -o build/test_GPFormulaTest.o -c test/GPFormulaTest.cpp -Iinclude
  
build/test_GPGoldenDividerTest.o : test/GPGoldenDividerTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -fPIC -o build/test_GPGoldenDividerTest.o -c test/GPGoldenDividerTest.cpp -Iinclude
  
build/test_GPGPDataTest.o : test/GPGPDataTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -fPIC -o build/test_GPGPDataTest.o -c test/GPGPDataTest.cpp -Iinclude
  
build/test_GPGraphicTest.o : test/GPGraphicTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -fPIC -o build/test_GPGraphicTest.o -c test/GPGraphicTest.cpp -Iinclude
  
build/test_GPloadXmlTreeTest.o : test/GPloadXmlTreeTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -fPIC -o build/test_GPloadXmlTreeTest.o -c test/GPloadXmlTreeTest.cpp -Iinclude
  
build/test_GPNetSearchTest.o : test/GPNetSearchTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -fPIC -o build/test_GPNetSearchTest.o -c test/GPNetSearchTest.cpp -Iinclude
  
build/test_GPPSOTest.o : test/GPPSOTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -fPIC -o build/test_GPPSOTest.o -c test/GPPSOTest.cpp -Iinclude
  
build/test_GPSearchIOTest.o : test/GPSearchIOTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -fPIC -o build/test_GPSearchIOTest.o -c test/GPSearchIOTest.cpp -Iinclude
  
build/test_GPSingleFunctionOptTest.o : test/GPSingleFunctionOptTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -fPIC -o build/test_GPSingleFunctionOptTest.o -c test/GPSingleFunctionOptTest.cpp -Iinclude
  
build/test_GPSingleTreeTest.o : test/GPSingleTreeTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -fPIC -o build/test_GPSingleTreeTest.o -c test/GPSingleTreeTest.cpp -Iinclude
  
build/test_GPTest.o : test/GPTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -fPIC -o build/test_GPTest.o -c test/GPTest.cpp -Iinclude
  
build/test_GPXmlEvolutionTest.o : test/GPXmlEvolutionTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -fPIC -o build/test_GPXmlEvolutionTest.o -c test/GPXmlEvolutionTest.cpp -Iinclude
  
build/test_GPxmlTreeTest.o : test/GPxmlTreeTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -fPIC -o build/test_GPxmlTreeTest.o -c test/GPxmlTreeTest.cpp -Iinclude
  
build/test_main.o : test/main.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -fPIC -o build/test_main.o -c test/main.cpp -Iinclude
  

ALL_INCLUESgpsuit.out= include//core/BasicComposeFunction.h include//core/genetic_operator.h include//core/GP_XmlString.h include//core/GPComputePoint.h include//core/GPData.h include//core/GPFactory.h include//core/GPFunctionDataBase.h include//core/GPProducer.h include//core/GPStatusContent.h include//core/group.h include//core/IDataBase.h include//core/IGPAutoDefFunction.h include//core/xmlGPDataLoader.h include//evolution/GPEvolutionGroup.h include//head.h include//math/AbstractPoint.h include//math/carryArray.h include//math/carryGroup2.h include//math/carryPoint.h include//math/carryTree.h include//math/FormulaTree.h include//math/GPSingleTree.h include//optimizor/GPGoldenDivideOpt.h include//optimizor/GPNetSearchOpt.h include//optimizor/GPOptimizorFactory.h include//optimizor/GPParticleSwarmOpt.h include//optimizor/IGPOptimizor.h include//producer/GPGraphicADF.h include//producer/GPGraphicProducer.h include//producer/GPTreeADF.h include//producer/GPTreeProducer.h include//system/system_lib.h include//user/adapter.h include//user/GPAPI.h include//user/GPContents.h include//user/IFunctionTable.h include//user/IStatusType.h include//utils/AutoClean.h include//utils/AutoStorage.h include//utils/BasicTree.h include//utils/debug.h include//utils/GP_Clock.h include//utils/GPRandom.h include//utils/RefCount.h include//xml/xmlReader.h include//xml/xmlTree.h

gpsuit.out:  build/GPSuit_FilePath.o build/GPSuit_main.o libGP.so
	g++ -std=c++11  -O3 -fPIC   ./libGP.so -o gpsuit.out  build/GPSuit_FilePath.o build/GPSuit_main.o ${SELF_VARIABLES}
build/GPSuit_FilePath.o : GPSuit/FilePath.cpp libGP.so  ${ALL_INCLUESgpsuit.out}
	g++ -std=c++11 -O3 -fPIC -o build/GPSuit_FilePath.o -c GPSuit/FilePath.cpp -Iinclude
  
build/GPSuit_main.o : GPSuit/main.cpp libGP.so  ${ALL_INCLUESgpsuit.out}
	g++ -std=c++11 -O3 -fPIC -o build/GPSuit_main.o -c GPSuit/main.cpp -Iinclude
  

ALL_INCLUESexample.out=

example.out:  build/simple_main.o libGP.so
	g++ -std=c++11  -O3 -fPIC   ./libGP.so -ldl -lpthread -o example.out  build/simple_main.o ${SELF_VARIABLES}
build/simple_main.o : simple_main.cpp libGP.so  ${ALL_INCLUESexample.out}
	g++ -std=c++11 -O3 -fPIC -o build/simple_main.o -c simple_main.cpp -Iinclude
  

ALL_INCLUESgptool.out= include//core/BasicComposeFunction.h include//core/genetic_operator.h include//core/GP_XmlString.h include//core/GPComputePoint.h include//core/GPData.h include//core/GPFactory.h include//core/GPFunctionDataBase.h include//core/GPProducer.h include//core/GPStatusContent.h include//core/group.h include//core/IDataBase.h include//core/IGPAutoDefFunction.h include//core/xmlGPDataLoader.h include//evolution/GPEvolutionGroup.h include//head.h include//math/AbstractPoint.h include//math/carryArray.h include//math/carryGroup2.h include//math/carryPoint.h include//math/carryTree.h include//math/FormulaTree.h include//math/GPSingleTree.h include//optimizor/GPGoldenDivideOpt.h include//optimizor/GPNetSearchOpt.h include//optimizor/GPOptimizorFactory.h include//optimizor/GPParticleSwarmOpt.h include//optimizor/IGPOptimizor.h include//producer/GPGraphicADF.h include//producer/GPGraphicProducer.h include//producer/GPTreeADF.h include//producer/GPTreeProducer.h include//system/system_lib.h include//user/adapter.h include//user/GPAPI.h include//user/GPContents.h include//user/IFunctionTable.h include//user/IStatusType.h include//utils/AutoClean.h include//utils/AutoStorage.h include//utils/BasicTree.h include//utils/debug.h include//utils/GP_Clock.h include//utils/GPRandom.h include//utils/RefCount.h include//xml/xmlReader.h include//xml/xmlTree.h

gptool.out:  build/gptool_main.o libGP.so
	g++ -std=c++11  -O3 -fPIC   ./libGP.so -ldl -lpthread -o gptool.out  build/gptool_main.o ${SELF_VARIABLES}
build/gptool_main.o : gptool/main.cpp libGP.so  ${ALL_INCLUESgptool.out}
	g++ -std=c++11 -O3 -fPIC -o build/gptool_main.o -c gptool/main.cpp -Iinclude
  

clean:
	rm build/*.o
	rm libGP.a
	rm libGP.so
	rm test.out
	rm gpsuit.out
	rm example.out
	rm gptool.out