all:libGP.a libGP.so test.out example.out 

ALL_INCLUESlibGP.a= include//backend/GPBackEndProducer.h include//backend/GPBasicPiecesFunctionCreator.h include//backend/GPComputePoint.h include//backend/GPDAGPiecesFunctionCreator.h include//backend/GPPiecesFunctionCreator.h include//backend/GPSingleParallelMachine.h include//backend/GPStreamADFProducer.h include//backend/GPTreeADF.h include//backend/GPTreePiecesFunctionCreator.h include//backend/GPTreeProducer.h include//core/GPFactory.h include//core/GPFunction.h include//core/GPFunctionDataBase.h include//core/GPFunctionTree.h include//core/GPMultiTable.h include//core/GPParallelMachineSet.h include//core/GPParameter.h include//core/GPPieceFactory.h include//core/GPProducer.h include//core/GPStatusContent.h include//core/GPStreamFactory.h include//core/GPStreamUtils.h include//core/IGPAutoDefFunction.h include//evolution/genetic_operator.h include//evolution/GPEvolutionGroup.h include//evolution/group.h include//frontend/GPFormulaTree.h include//frontend/GPFrontEndProducer.h include//frontend/GPFunctionFrontEndProducer.h include//frontend/GPProducerUtils.h include//GPStrings.h include//head.h include//lowlevelAPI/GPContents.h include//lowlevelAPI/GPParallelType.h include//lowlevelAPI/GPPieces.h include//lowlevelAPI/GPPiecesFunction.h include//lowlevelAPI/GPRefCount.h include//lowlevelAPI/GPStream.h include//lowlevelAPI/GPTypes.h include//lowlevelAPI/IFunctionTable.h include//lowlevelAPI/IGPFunction.h include//lowlevelAPI/IParallelMachine.h include//lowlevelAPI/IStatusType.h include//math/carryArray.h include//math/carryGroup.h include//math/carryPoint.h include//math/carryTree.h include//math/GPAbstractPoint.h include//math/GPCarryVaryGroup.h include//math/GPSingleTree.h include//math/GPSingleTreeFunction.h include//midend/GPMapReduceMerger.h include//midend/GPMultiLayerTree.h include//optimizor/GPGoldenDivideOpt.h include//optimizor/GPNetSearchOpt.h include//optimizor/GPOptimizorFactory.h include//optimizor/GPParticleSwarmOpt.h include//optimizor/IGPOptimizor.h include//platform/GPSystem.h include//platform/system_lib.h include//user/GPAPI.h include//utils/AutoClean.h include//utils/AutoStorage.h include//utils/GPBlock.h include//utils/GPClock.h include//utils/GPDebug.h include//utils/GPRandom.h include//utils/GPStringHelper.h include//utils/GPTreeNode.h include//xml/xmlReader.h

libGP.a:  build/src_backend_GPBasicPiecesFunctionCreator_cpp.o build/src_backend_GPComputePoint_cpp.o build/src_backend_GPDAGPiecesFunction_cpp.o build/src_backend_GPDAGPiecesFunctionCreator_cpp.o build/src_backend_GPSingleParallelMachine_cpp.o build/src_backend_GPStreamADF_cpp.o build/src_backend_GPStreamADFProducer_cpp.o build/src_backend_GPTreeADF_cpp.o build/src_backend_GPTreePiecesFunctionCreator_cpp.o build/src_backend_GPTreeProducer_cpp.o build/src_core_GPDoubleType_cpp.o build/src_core_GPFactory_cpp.o build/src_core_GPFileStream_cpp.o build/src_core_GPFunctionDataBase_cpp.o build/src_core_GPFunctionTree_cpp.o build/src_core_GPMultiTable_cpp.o build/src_core_GPParallelMachineSet_cpp.o build/src_core_GPParameter_cpp.o build/src_core_GPPieceFactory_cpp.o build/src_core_GPProducer_cpp.o build/src_core_GPStatusContent_cpp.o build/src_core_GPStlStream_cpp.o build/src_core_GPStreamFactory_cpp.o build/src_core_GPStreamUtils_cpp.o build/src_core_GPStringType_cpp.o build/src_core_GPUserStream_cpp.o build/src_evolution_GPEvolutionGroup_cpp.o build/src_frontend_GPFormulaTree_cpp.o build/src_frontend_GPFunctionFrontEndProducer_cpp.o build/src_frontend_GPProducerUtils_cpp.o build/src_frontend_recurse_tree_cpp.o build/src_GPStrings_cpp.o build/src_math_carryArray_cpp.o build/src_math_carryPoint_cpp.o build/src_math_GPAbstractPoint_cpp.o build/src_math_GPCarryVaryGroup_cpp.o build/src_math_GPSingleTree_cpp.o build/src_math_GPSingleTreeFunction_cpp.o build/src_midend_GPMapReduceMerger_cpp.o build/src_midend_GPMultiLayerTree_cpp.o build/src_optimizor_GPGoldenDivideOpt_cpp.o build/src_optimizor_GPNetSearchOpt_cpp.o build/src_optimizor_GPOptimizorFactory_cpp.o build/src_optimizor_GPParticleSwarmOpt_cpp.o build/src_platform_GPSystem_cpp.o build/src_user_GPAPI_cpp.o build/src_utils_GPBlock_cpp.o build/src_utils_GPClock_cpp.o build/src_utils_GPRandom_cpp.o build/src_utils_GPStringHelper_cpp.o build/src_utils_GPTreeNode_cpp.o build/src_xml_xmlReader_cpp.o 
	rm -f libGP.a
	ar rcs libGP.a  build/src_backend_GPBasicPiecesFunctionCreator_cpp.o build/src_backend_GPComputePoint_cpp.o build/src_backend_GPDAGPiecesFunction_cpp.o build/src_backend_GPDAGPiecesFunctionCreator_cpp.o build/src_backend_GPSingleParallelMachine_cpp.o build/src_backend_GPStreamADF_cpp.o build/src_backend_GPStreamADFProducer_cpp.o build/src_backend_GPTreeADF_cpp.o build/src_backend_GPTreePiecesFunctionCreator_cpp.o build/src_backend_GPTreeProducer_cpp.o build/src_core_GPDoubleType_cpp.o build/src_core_GPFactory_cpp.o build/src_core_GPFileStream_cpp.o build/src_core_GPFunctionDataBase_cpp.o build/src_core_GPFunctionTree_cpp.o build/src_core_GPMultiTable_cpp.o build/src_core_GPParallelMachineSet_cpp.o build/src_core_GPParameter_cpp.o build/src_core_GPPieceFactory_cpp.o build/src_core_GPProducer_cpp.o build/src_core_GPStatusContent_cpp.o build/src_core_GPStlStream_cpp.o build/src_core_GPStreamFactory_cpp.o build/src_core_GPStreamUtils_cpp.o build/src_core_GPStringType_cpp.o build/src_core_GPUserStream_cpp.o build/src_evolution_GPEvolutionGroup_cpp.o build/src_frontend_GPFormulaTree_cpp.o build/src_frontend_GPFunctionFrontEndProducer_cpp.o build/src_frontend_GPProducerUtils_cpp.o build/src_frontend_recurse_tree_cpp.o build/src_GPStrings_cpp.o build/src_math_carryArray_cpp.o build/src_math_carryPoint_cpp.o build/src_math_GPAbstractPoint_cpp.o build/src_math_GPCarryVaryGroup_cpp.o build/src_math_GPSingleTree_cpp.o build/src_math_GPSingleTreeFunction_cpp.o build/src_midend_GPMapReduceMerger_cpp.o build/src_midend_GPMultiLayerTree_cpp.o build/src_optimizor_GPGoldenDivideOpt_cpp.o build/src_optimizor_GPNetSearchOpt_cpp.o build/src_optimizor_GPOptimizorFactory_cpp.o build/src_optimizor_GPParticleSwarmOpt_cpp.o build/src_platform_GPSystem_cpp.o build/src_user_GPAPI_cpp.o build/src_utils_GPBlock_cpp.o build/src_utils_GPClock_cpp.o build/src_utils_GPRandom_cpp.o build/src_utils_GPStringHelper_cpp.o build/src_utils_GPTreeNode_cpp.o build/src_xml_xmlReader_cpp.o ${SELF_VARIABLES}
build/src_backend_GPBasicPiecesFunctionCreator_cpp.o : src/backend/GPBasicPiecesFunctionCreator.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_backend_GPBasicPiecesFunctionCreator_cpp.o -c src/backend/GPBasicPiecesFunctionCreator.cpp -Iinclude
  
build/src_backend_GPComputePoint_cpp.o : src/backend/GPComputePoint.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_backend_GPComputePoint_cpp.o -c src/backend/GPComputePoint.cpp -Iinclude
  
build/src_backend_GPDAGPiecesFunction_cpp.o : src/backend/GPDAGPiecesFunction.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_backend_GPDAGPiecesFunction_cpp.o -c src/backend/GPDAGPiecesFunction.cpp -Iinclude
  
build/src_backend_GPDAGPiecesFunctionCreator_cpp.o : src/backend/GPDAGPiecesFunctionCreator.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_backend_GPDAGPiecesFunctionCreator_cpp.o -c src/backend/GPDAGPiecesFunctionCreator.cpp -Iinclude
  
build/src_backend_GPSingleParallelMachine_cpp.o : src/backend/GPSingleParallelMachine.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_backend_GPSingleParallelMachine_cpp.o -c src/backend/GPSingleParallelMachine.cpp -Iinclude
  
build/src_backend_GPStreamADF_cpp.o : src/backend/GPStreamADF.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_backend_GPStreamADF_cpp.o -c src/backend/GPStreamADF.cpp -Iinclude
  
build/src_backend_GPStreamADFProducer_cpp.o : src/backend/GPStreamADFProducer.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_backend_GPStreamADFProducer_cpp.o -c src/backend/GPStreamADFProducer.cpp -Iinclude
  
build/src_backend_GPTreeADF_cpp.o : src/backend/GPTreeADF.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_backend_GPTreeADF_cpp.o -c src/backend/GPTreeADF.cpp -Iinclude
  
build/src_backend_GPTreePiecesFunctionCreator_cpp.o : src/backend/GPTreePiecesFunctionCreator.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_backend_GPTreePiecesFunctionCreator_cpp.o -c src/backend/GPTreePiecesFunctionCreator.cpp -Iinclude
  
build/src_backend_GPTreeProducer_cpp.o : src/backend/GPTreeProducer.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_backend_GPTreeProducer_cpp.o -c src/backend/GPTreeProducer.cpp -Iinclude
  
build/src_core_GPDoubleType_cpp.o : src/core/GPDoubleType.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_core_GPDoubleType_cpp.o -c src/core/GPDoubleType.cpp -Iinclude
  
build/src_core_GPFactory_cpp.o : src/core/GPFactory.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_core_GPFactory_cpp.o -c src/core/GPFactory.cpp -Iinclude
  
build/src_core_GPFileStream_cpp.o : src/core/GPFileStream.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_core_GPFileStream_cpp.o -c src/core/GPFileStream.cpp -Iinclude
  
build/src_core_GPFunctionDataBase_cpp.o : src/core/GPFunctionDataBase.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_core_GPFunctionDataBase_cpp.o -c src/core/GPFunctionDataBase.cpp -Iinclude
  
build/src_core_GPFunctionTree_cpp.o : src/core/GPFunctionTree.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_core_GPFunctionTree_cpp.o -c src/core/GPFunctionTree.cpp -Iinclude
  
build/src_core_GPMultiTable_cpp.o : src/core/GPMultiTable.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_core_GPMultiTable_cpp.o -c src/core/GPMultiTable.cpp -Iinclude
  
build/src_core_GPParallelMachineSet_cpp.o : src/core/GPParallelMachineSet.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_core_GPParallelMachineSet_cpp.o -c src/core/GPParallelMachineSet.cpp -Iinclude
  
build/src_core_GPParameter_cpp.o : src/core/GPParameter.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_core_GPParameter_cpp.o -c src/core/GPParameter.cpp -Iinclude
  
build/src_core_GPPieceFactory_cpp.o : src/core/GPPieceFactory.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_core_GPPieceFactory_cpp.o -c src/core/GPPieceFactory.cpp -Iinclude
  
build/src_core_GPProducer_cpp.o : src/core/GPProducer.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_core_GPProducer_cpp.o -c src/core/GPProducer.cpp -Iinclude
  
build/src_core_GPStatusContent_cpp.o : src/core/GPStatusContent.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_core_GPStatusContent_cpp.o -c src/core/GPStatusContent.cpp -Iinclude
  
build/src_core_GPStlStream_cpp.o : src/core/GPStlStream.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_core_GPStlStream_cpp.o -c src/core/GPStlStream.cpp -Iinclude
  
build/src_core_GPStreamFactory_cpp.o : src/core/GPStreamFactory.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_core_GPStreamFactory_cpp.o -c src/core/GPStreamFactory.cpp -Iinclude
  
build/src_core_GPStreamUtils_cpp.o : src/core/GPStreamUtils.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_core_GPStreamUtils_cpp.o -c src/core/GPStreamUtils.cpp -Iinclude
  
build/src_core_GPStringType_cpp.o : src/core/GPStringType.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_core_GPStringType_cpp.o -c src/core/GPStringType.cpp -Iinclude
  
build/src_core_GPUserStream_cpp.o : src/core/GPUserStream.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_core_GPUserStream_cpp.o -c src/core/GPUserStream.cpp -Iinclude
  
build/src_evolution_GPEvolutionGroup_cpp.o : src/evolution/GPEvolutionGroup.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_evolution_GPEvolutionGroup_cpp.o -c src/evolution/GPEvolutionGroup.cpp -Iinclude
  
build/src_frontend_GPFormulaTree_cpp.o : src/frontend/GPFormulaTree.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_frontend_GPFormulaTree_cpp.o -c src/frontend/GPFormulaTree.cpp -Iinclude
  
build/src_frontend_GPFunctionFrontEndProducer_cpp.o : src/frontend/GPFunctionFrontEndProducer.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_frontend_GPFunctionFrontEndProducer_cpp.o -c src/frontend/GPFunctionFrontEndProducer.cpp -Iinclude
  
build/src_frontend_GPProducerUtils_cpp.o : src/frontend/GPProducerUtils.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_frontend_GPProducerUtils_cpp.o -c src/frontend/GPProducerUtils.cpp -Iinclude
  
build/src_frontend_recurse_tree_cpp.o : src/frontend/recurse_tree.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_frontend_recurse_tree_cpp.o -c src/frontend/recurse_tree.cpp -Iinclude
  
build/src_GPStrings_cpp.o : src/GPStrings.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_GPStrings_cpp.o -c src/GPStrings.cpp -Iinclude
  
build/src_math_carryArray_cpp.o : src/math/carryArray.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_math_carryArray_cpp.o -c src/math/carryArray.cpp -Iinclude
  
build/src_math_carryPoint_cpp.o : src/math/carryPoint.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_math_carryPoint_cpp.o -c src/math/carryPoint.cpp -Iinclude
  
build/src_math_GPAbstractPoint_cpp.o : src/math/GPAbstractPoint.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_math_GPAbstractPoint_cpp.o -c src/math/GPAbstractPoint.cpp -Iinclude
  
build/src_math_GPCarryVaryGroup_cpp.o : src/math/GPCarryVaryGroup.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_math_GPCarryVaryGroup_cpp.o -c src/math/GPCarryVaryGroup.cpp -Iinclude
  
build/src_math_GPSingleTree_cpp.o : src/math/GPSingleTree.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_math_GPSingleTree_cpp.o -c src/math/GPSingleTree.cpp -Iinclude
  
build/src_math_GPSingleTreeFunction_cpp.o : src/math/GPSingleTreeFunction.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_math_GPSingleTreeFunction_cpp.o -c src/math/GPSingleTreeFunction.cpp -Iinclude
  
build/src_midend_GPMapReduceMerger_cpp.o : src/midend/GPMapReduceMerger.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_midend_GPMapReduceMerger_cpp.o -c src/midend/GPMapReduceMerger.cpp -Iinclude
  
build/src_midend_GPMultiLayerTree_cpp.o : src/midend/GPMultiLayerTree.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_midend_GPMultiLayerTree_cpp.o -c src/midend/GPMultiLayerTree.cpp -Iinclude
  
build/src_optimizor_GPGoldenDivideOpt_cpp.o : src/optimizor/GPGoldenDivideOpt.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_optimizor_GPGoldenDivideOpt_cpp.o -c src/optimizor/GPGoldenDivideOpt.cpp -Iinclude
  
build/src_optimizor_GPNetSearchOpt_cpp.o : src/optimizor/GPNetSearchOpt.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_optimizor_GPNetSearchOpt_cpp.o -c src/optimizor/GPNetSearchOpt.cpp -Iinclude
  
build/src_optimizor_GPOptimizorFactory_cpp.o : src/optimizor/GPOptimizorFactory.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_optimizor_GPOptimizorFactory_cpp.o -c src/optimizor/GPOptimizorFactory.cpp -Iinclude
  
build/src_optimizor_GPParticleSwarmOpt_cpp.o : src/optimizor/GPParticleSwarmOpt.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_optimizor_GPParticleSwarmOpt_cpp.o -c src/optimizor/GPParticleSwarmOpt.cpp -Iinclude
  
build/src_platform_GPSystem_cpp.o : src/platform/GPSystem.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_platform_GPSystem_cpp.o -c src/platform/GPSystem.cpp -Iinclude
  
build/src_user_GPAPI_cpp.o : src/user/GPAPI.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_user_GPAPI_cpp.o -c src/user/GPAPI.cpp -Iinclude
  
build/src_utils_GPBlock_cpp.o : src/utils/GPBlock.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_utils_GPBlock_cpp.o -c src/utils/GPBlock.cpp -Iinclude
  
build/src_utils_GPClock_cpp.o : src/utils/GPClock.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_utils_GPClock_cpp.o -c src/utils/GPClock.cpp -Iinclude
  
build/src_utils_GPRandom_cpp.o : src/utils/GPRandom.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_utils_GPRandom_cpp.o -c src/utils/GPRandom.cpp -Iinclude
  
build/src_utils_GPStringHelper_cpp.o : src/utils/GPStringHelper.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_utils_GPStringHelper_cpp.o -c src/utils/GPStringHelper.cpp -Iinclude
  
build/src_utils_GPTreeNode_cpp.o : src/utils/GPTreeNode.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_utils_GPTreeNode_cpp.o -c src/utils/GPTreeNode.cpp -Iinclude
  
build/src_xml_xmlReader_cpp.o : src/xml/xmlReader.cpp   ${ALL_INCLUESlibGP.a}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_xml_xmlReader_cpp.o -c src/xml/xmlReader.cpp -Iinclude
  

ALL_INCLUESlibGP.so= include//backend/GPBackEndProducer.h include//backend/GPBasicPiecesFunctionCreator.h include//backend/GPComputePoint.h include//backend/GPDAGPiecesFunctionCreator.h include//backend/GPPiecesFunctionCreator.h include//backend/GPSingleParallelMachine.h include//backend/GPStreamADFProducer.h include//backend/GPTreeADF.h include//backend/GPTreePiecesFunctionCreator.h include//backend/GPTreeProducer.h include//core/GPFactory.h include//core/GPFunction.h include//core/GPFunctionDataBase.h include//core/GPFunctionTree.h include//core/GPMultiTable.h include//core/GPParallelMachineSet.h include//core/GPParameter.h include//core/GPPieceFactory.h include//core/GPProducer.h include//core/GPStatusContent.h include//core/GPStreamFactory.h include//core/GPStreamUtils.h include//core/IGPAutoDefFunction.h include//evolution/genetic_operator.h include//evolution/GPEvolutionGroup.h include//evolution/group.h include//frontend/GPFormulaTree.h include//frontend/GPFrontEndProducer.h include//frontend/GPFunctionFrontEndProducer.h include//frontend/GPProducerUtils.h include//GPStrings.h include//head.h include//lowlevelAPI/GPContents.h include//lowlevelAPI/GPParallelType.h include//lowlevelAPI/GPPieces.h include//lowlevelAPI/GPPiecesFunction.h include//lowlevelAPI/GPRefCount.h include//lowlevelAPI/GPStream.h include//lowlevelAPI/GPTypes.h include//lowlevelAPI/IFunctionTable.h include//lowlevelAPI/IGPFunction.h include//lowlevelAPI/IParallelMachine.h include//lowlevelAPI/IStatusType.h include//math/carryArray.h include//math/carryGroup.h include//math/carryPoint.h include//math/carryTree.h include//math/GPAbstractPoint.h include//math/GPCarryVaryGroup.h include//math/GPSingleTree.h include//math/GPSingleTreeFunction.h include//midend/GPMapReduceMerger.h include//midend/GPMultiLayerTree.h include//optimizor/GPGoldenDivideOpt.h include//optimizor/GPNetSearchOpt.h include//optimizor/GPOptimizorFactory.h include//optimizor/GPParticleSwarmOpt.h include//optimizor/IGPOptimizor.h include//platform/GPSystem.h include//platform/system_lib.h include//user/GPAPI.h include//utils/AutoClean.h include//utils/AutoStorage.h include//utils/GPBlock.h include//utils/GPClock.h include//utils/GPDebug.h include//utils/GPRandom.h include//utils/GPStringHelper.h include//utils/GPTreeNode.h include//xml/xmlReader.h

libGP.so:  build/src_backend_GPBasicPiecesFunctionCreator_cpp.o build/src_backend_GPComputePoint_cpp.o build/src_backend_GPDAGPiecesFunction_cpp.o build/src_backend_GPDAGPiecesFunctionCreator_cpp.o build/src_backend_GPSingleParallelMachine_cpp.o build/src_backend_GPStreamADF_cpp.o build/src_backend_GPStreamADFProducer_cpp.o build/src_backend_GPTreeADF_cpp.o build/src_backend_GPTreePiecesFunctionCreator_cpp.o build/src_backend_GPTreeProducer_cpp.o build/src_core_GPDoubleType_cpp.o build/src_core_GPFactory_cpp.o build/src_core_GPFileStream_cpp.o build/src_core_GPFunctionDataBase_cpp.o build/src_core_GPFunctionTree_cpp.o build/src_core_GPMultiTable_cpp.o build/src_core_GPParallelMachineSet_cpp.o build/src_core_GPParameter_cpp.o build/src_core_GPPieceFactory_cpp.o build/src_core_GPProducer_cpp.o build/src_core_GPStatusContent_cpp.o build/src_core_GPStlStream_cpp.o build/src_core_GPStreamFactory_cpp.o build/src_core_GPStreamUtils_cpp.o build/src_core_GPStringType_cpp.o build/src_core_GPUserStream_cpp.o build/src_evolution_GPEvolutionGroup_cpp.o build/src_frontend_GPFormulaTree_cpp.o build/src_frontend_GPFunctionFrontEndProducer_cpp.o build/src_frontend_GPProducerUtils_cpp.o build/src_frontend_recurse_tree_cpp.o build/src_GPStrings_cpp.o build/src_math_carryArray_cpp.o build/src_math_carryPoint_cpp.o build/src_math_GPAbstractPoint_cpp.o build/src_math_GPCarryVaryGroup_cpp.o build/src_math_GPSingleTree_cpp.o build/src_math_GPSingleTreeFunction_cpp.o build/src_midend_GPMapReduceMerger_cpp.o build/src_midend_GPMultiLayerTree_cpp.o build/src_optimizor_GPGoldenDivideOpt_cpp.o build/src_optimizor_GPNetSearchOpt_cpp.o build/src_optimizor_GPOptimizorFactory_cpp.o build/src_optimizor_GPParticleSwarmOpt_cpp.o build/src_platform_GPSystem_cpp.o build/src_user_GPAPI_cpp.o build/src_utils_GPBlock_cpp.o build/src_utils_GPClock_cpp.o build/src_utils_GPRandom_cpp.o build/src_utils_GPStringHelper_cpp.o build/src_utils_GPTreeNode_cpp.o build/src_xml_xmlReader_cpp.o 
	g++ -std=c++11   build/src_backend_GPBasicPiecesFunctionCreator_cpp.o build/src_backend_GPComputePoint_cpp.o build/src_backend_GPDAGPiecesFunction_cpp.o build/src_backend_GPDAGPiecesFunctionCreator_cpp.o build/src_backend_GPSingleParallelMachine_cpp.o build/src_backend_GPStreamADF_cpp.o build/src_backend_GPStreamADFProducer_cpp.o build/src_backend_GPTreeADF_cpp.o build/src_backend_GPTreePiecesFunctionCreator_cpp.o build/src_backend_GPTreeProducer_cpp.o build/src_core_GPDoubleType_cpp.o build/src_core_GPFactory_cpp.o build/src_core_GPFileStream_cpp.o build/src_core_GPFunctionDataBase_cpp.o build/src_core_GPFunctionTree_cpp.o build/src_core_GPMultiTable_cpp.o build/src_core_GPParallelMachineSet_cpp.o build/src_core_GPParameter_cpp.o build/src_core_GPPieceFactory_cpp.o build/src_core_GPProducer_cpp.o build/src_core_GPStatusContent_cpp.o build/src_core_GPStlStream_cpp.o build/src_core_GPStreamFactory_cpp.o build/src_core_GPStreamUtils_cpp.o build/src_core_GPStringType_cpp.o build/src_core_GPUserStream_cpp.o build/src_evolution_GPEvolutionGroup_cpp.o build/src_frontend_GPFormulaTree_cpp.o build/src_frontend_GPFunctionFrontEndProducer_cpp.o build/src_frontend_GPProducerUtils_cpp.o build/src_frontend_recurse_tree_cpp.o build/src_GPStrings_cpp.o build/src_math_carryArray_cpp.o build/src_math_carryPoint_cpp.o build/src_math_GPAbstractPoint_cpp.o build/src_math_GPCarryVaryGroup_cpp.o build/src_math_GPSingleTree_cpp.o build/src_math_GPSingleTreeFunction_cpp.o build/src_midend_GPMapReduceMerger_cpp.o build/src_midend_GPMultiLayerTree_cpp.o build/src_optimizor_GPGoldenDivideOpt_cpp.o build/src_optimizor_GPNetSearchOpt_cpp.o build/src_optimizor_GPOptimizorFactory_cpp.o build/src_optimizor_GPParticleSwarmOpt_cpp.o build/src_platform_GPSystem_cpp.o build/src_user_GPAPI_cpp.o build/src_utils_GPBlock_cpp.o build/src_utils_GPClock_cpp.o build/src_utils_GPRandom_cpp.o build/src_utils_GPStringHelper_cpp.o build/src_utils_GPTreeNode_cpp.o build/src_xml_xmlReader_cpp.o -O3 -g -fPIC  --shared  -lpthread -ldl -o libGP.so ${SELF_VARIABLES}

ALL_INCLUEStest.out= include//backend/GPBackEndProducer.h include//backend/GPBasicPiecesFunctionCreator.h include//backend/GPComputePoint.h include//backend/GPDAGPiecesFunctionCreator.h include//backend/GPPiecesFunctionCreator.h include//backend/GPSingleParallelMachine.h include//backend/GPStreamADFProducer.h include//backend/GPTreeADF.h include//backend/GPTreePiecesFunctionCreator.h include//backend/GPTreeProducer.h include//core/GPFactory.h include//core/GPFunction.h include//core/GPFunctionDataBase.h include//core/GPFunctionTree.h include//core/GPMultiTable.h include//core/GPParallelMachineSet.h include//core/GPParameter.h include//core/GPPieceFactory.h include//core/GPProducer.h include//core/GPStatusContent.h include//core/GPStreamFactory.h include//core/GPStreamUtils.h include//core/IGPAutoDefFunction.h include//evolution/genetic_operator.h include//evolution/GPEvolutionGroup.h include//evolution/group.h include//frontend/GPFormulaTree.h include//frontend/GPFrontEndProducer.h include//frontend/GPFunctionFrontEndProducer.h include//frontend/GPProducerUtils.h include//GPStrings.h include//head.h include//lowlevelAPI/GPContents.h include//lowlevelAPI/GPParallelType.h include//lowlevelAPI/GPPieces.h include//lowlevelAPI/GPPiecesFunction.h include//lowlevelAPI/GPRefCount.h include//lowlevelAPI/GPStream.h include//lowlevelAPI/GPTypes.h include//lowlevelAPI/IFunctionTable.h include//lowlevelAPI/IGPFunction.h include//lowlevelAPI/IParallelMachine.h include//lowlevelAPI/IStatusType.h include//math/carryArray.h include//math/carryGroup.h include//math/carryPoint.h include//math/carryTree.h include//math/GPAbstractPoint.h include//math/GPCarryVaryGroup.h include//math/GPSingleTree.h include//math/GPSingleTreeFunction.h include//midend/GPMapReduceMerger.h include//midend/GPMultiLayerTree.h include//optimizor/GPGoldenDivideOpt.h include//optimizor/GPNetSearchOpt.h include//optimizor/GPOptimizorFactory.h include//optimizor/GPParticleSwarmOpt.h include//optimizor/IGPOptimizor.h include//platform/GPSystem.h include//platform/system_lib.h include//user/GPAPI.h include//utils/AutoClean.h include//utils/AutoStorage.h include//utils/GPBlock.h include//utils/GPClock.h include//utils/GPDebug.h include//utils/GPRandom.h include//utils/GPStringHelper.h include//utils/GPTreeNode.h include//xml/xmlReader.h

test.out:  build/test_AbstractPointTest_cpp.o build/test_GPAPITest_cpp.o build/test_GPBasicPiecesFunctionCreatorTest_cpp.o build/test_GPcarryGroupTest_cpp.o build/test_GPCarryVaryGroupTest_cpp.o build/test_GPFormulaADFTest_cpp.o build/test_GPFormulaTreeTest_cpp.o build/test_GPFrontEndProducerTest_cpp.o build/test_GPGoldenDividerTest_cpp.o build/test_GPMultiLayerTreeTest_cpp.o build/test_GPMultiOutputTest_cpp.o build/test_GPNetSearchTest_cpp.o build/test_GPParallelTest_cpp.o build/test_GPPiecesFunctionFactoryTest_cpp.o build/test_GPPSOTest_cpp.o build/test_GPSearchIOTest_cpp.o build/test_GPSingleParallelMachineTest_cpp.o build/test_GPSingleTreeTest_cpp.o build/test_GPStreamADFTest_cpp.o build/test_GPStreamTest_cpp.o build/test_GPSystemTest_cpp.o build/test_GPTest_cpp.o build/test_GPTestMain_cpp.o build/test_GPThreadParallelTest_cpp.o build/test_GPTreeProducerTest_cpp.o build/test_GPXmlEvolutionTest_cpp.o libGP.so
	g++ -std=c++11   build/test_AbstractPointTest_cpp.o build/test_GPAPITest_cpp.o build/test_GPBasicPiecesFunctionCreatorTest_cpp.o build/test_GPcarryGroupTest_cpp.o build/test_GPCarryVaryGroupTest_cpp.o build/test_GPFormulaADFTest_cpp.o build/test_GPFormulaTreeTest_cpp.o build/test_GPFrontEndProducerTest_cpp.o build/test_GPGoldenDividerTest_cpp.o build/test_GPMultiLayerTreeTest_cpp.o build/test_GPMultiOutputTest_cpp.o build/test_GPNetSearchTest_cpp.o build/test_GPParallelTest_cpp.o build/test_GPPiecesFunctionFactoryTest_cpp.o build/test_GPPSOTest_cpp.o build/test_GPSearchIOTest_cpp.o build/test_GPSingleParallelMachineTest_cpp.o build/test_GPSingleTreeTest_cpp.o build/test_GPStreamADFTest_cpp.o build/test_GPStreamTest_cpp.o build/test_GPSystemTest_cpp.o build/test_GPTest_cpp.o build/test_GPTestMain_cpp.o build/test_GPThreadParallelTest_cpp.o build/test_GPTreeProducerTest_cpp.o build/test_GPXmlEvolutionTest_cpp.o -O3 -g -fPIC   ./libGP.so -lpthread -ldl -o test.out ${SELF_VARIABLES}
build/test_AbstractPointTest_cpp.o : test/AbstractPointTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_AbstractPointTest_cpp.o -c test/AbstractPointTest.cpp -Iinclude
  
build/test_GPAPITest_cpp.o : test/GPAPITest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPAPITest_cpp.o -c test/GPAPITest.cpp -Iinclude
  
build/test_GPBasicPiecesFunctionCreatorTest_cpp.o : test/GPBasicPiecesFunctionCreatorTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPBasicPiecesFunctionCreatorTest_cpp.o -c test/GPBasicPiecesFunctionCreatorTest.cpp -Iinclude
  
build/test_GPcarryGroupTest_cpp.o : test/GPcarryGroupTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPcarryGroupTest_cpp.o -c test/GPcarryGroupTest.cpp -Iinclude
  
build/test_GPCarryVaryGroupTest_cpp.o : test/GPCarryVaryGroupTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPCarryVaryGroupTest_cpp.o -c test/GPCarryVaryGroupTest.cpp -Iinclude
  
build/test_GPFormulaADFTest_cpp.o : test/GPFormulaADFTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPFormulaADFTest_cpp.o -c test/GPFormulaADFTest.cpp -Iinclude
  
build/test_GPFormulaTreeTest_cpp.o : test/GPFormulaTreeTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPFormulaTreeTest_cpp.o -c test/GPFormulaTreeTest.cpp -Iinclude
  
build/test_GPFrontEndProducerTest_cpp.o : test/GPFrontEndProducerTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPFrontEndProducerTest_cpp.o -c test/GPFrontEndProducerTest.cpp -Iinclude
  
build/test_GPGoldenDividerTest_cpp.o : test/GPGoldenDividerTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPGoldenDividerTest_cpp.o -c test/GPGoldenDividerTest.cpp -Iinclude
  
build/test_GPMultiLayerTreeTest_cpp.o : test/GPMultiLayerTreeTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPMultiLayerTreeTest_cpp.o -c test/GPMultiLayerTreeTest.cpp -Iinclude
  
build/test_GPMultiOutputTest_cpp.o : test/GPMultiOutputTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPMultiOutputTest_cpp.o -c test/GPMultiOutputTest.cpp -Iinclude
  
build/test_GPNetSearchTest_cpp.o : test/GPNetSearchTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPNetSearchTest_cpp.o -c test/GPNetSearchTest.cpp -Iinclude
  
build/test_GPParallelTest_cpp.o : test/GPParallelTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPParallelTest_cpp.o -c test/GPParallelTest.cpp -Iinclude
  
build/test_GPPiecesFunctionFactoryTest_cpp.o : test/GPPiecesFunctionFactoryTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPPiecesFunctionFactoryTest_cpp.o -c test/GPPiecesFunctionFactoryTest.cpp -Iinclude
  
build/test_GPPSOTest_cpp.o : test/GPPSOTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPPSOTest_cpp.o -c test/GPPSOTest.cpp -Iinclude
  
build/test_GPSearchIOTest_cpp.o : test/GPSearchIOTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPSearchIOTest_cpp.o -c test/GPSearchIOTest.cpp -Iinclude
  
build/test_GPSingleParallelMachineTest_cpp.o : test/GPSingleParallelMachineTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPSingleParallelMachineTest_cpp.o -c test/GPSingleParallelMachineTest.cpp -Iinclude
  
build/test_GPSingleTreeTest_cpp.o : test/GPSingleTreeTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPSingleTreeTest_cpp.o -c test/GPSingleTreeTest.cpp -Iinclude
  
build/test_GPStreamADFTest_cpp.o : test/GPStreamADFTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPStreamADFTest_cpp.o -c test/GPStreamADFTest.cpp -Iinclude
  
build/test_GPStreamTest_cpp.o : test/GPStreamTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPStreamTest_cpp.o -c test/GPStreamTest.cpp -Iinclude
  
build/test_GPSystemTest_cpp.o : test/GPSystemTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPSystemTest_cpp.o -c test/GPSystemTest.cpp -Iinclude
  
build/test_GPTest_cpp.o : test/GPTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPTest_cpp.o -c test/GPTest.cpp -Iinclude
  
build/test_GPTestMain_cpp.o : test/GPTestMain.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPTestMain_cpp.o -c test/GPTestMain.cpp -Iinclude
  
build/test_GPThreadParallelTest_cpp.o : test/GPThreadParallelTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPThreadParallelTest_cpp.o -c test/GPThreadParallelTest.cpp -Iinclude
  
build/test_GPTreeProducerTest_cpp.o : test/GPTreeProducerTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPTreeProducerTest_cpp.o -c test/GPTreeProducerTest.cpp -Iinclude
  
build/test_GPXmlEvolutionTest_cpp.o : test/GPXmlEvolutionTest.cpp libGP.so  ${ALL_INCLUEStest.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/test_GPXmlEvolutionTest_cpp.o -c test/GPXmlEvolutionTest.cpp -Iinclude
  

ALL_INCLUESexample.out=

example.out:  build/simple_main_cpp.o libGP.a
	g++ -std=c++11   build/simple_main_cpp.o -O3 -g -fPIC   ./libGP.a -ldl -lpthread -o example.out ${SELF_VARIABLES}
build/simple_main_cpp.o : simple_main.cpp libGP.a  ${ALL_INCLUESexample.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/simple_main_cpp.o -c simple_main.cpp -Iinclude
  

clean:
	rm build/*.o
	rm libGP.a
	rm libGP.so
	rm test.out
	rm example.out