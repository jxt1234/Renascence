all:libmthpGP.so example.out 

ALL_INCLUESlibmthpGP.so= include//AutoStorage.h include//GPCarryVaryGroup.h include//GPClock.h include//MGPCreator.h include//MGPExecutor.h include//MGPMachine.h include//MGPPiecesFactory.h include//MGPThread.h include//MGPThreadPool.h include//MGPUtils.h

libmthpGP.so:  build/src_GPCarryVaryGroup.o build/src_GPClock.o build/src_MGPCreator.o build/src_MGPExecutor.o build/src_MGPKeyMatcher.o build/src_MGPMachine.o build/src_MGPPiecesFactory.o build/src_MGPThread.o build/src_MGPThread_Linux.o build/src_MGPThreadPool.o 
	g++ -std=c++11   build/src_GPCarryVaryGroup.o build/src_GPClock.o build/src_MGPCreator.o build/src_MGPExecutor.o build/src_MGPKeyMatcher.o build/src_MGPMachine.o build/src_MGPPiecesFactory.o build/src_MGPThread.o build/src_MGPThread_Linux.o build/src_MGPThreadPool.o -O3 -g -fPIC  --shared  -lpthread -ldl -o libmthpGP.so ${SELF_VARIABLES}
build/src_GPCarryVaryGroup.o : src/GPCarryVaryGroup.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_GPCarryVaryGroup.o -c src/GPCarryVaryGroup.cpp -Iinclude -I../../include/
  
build/src_GPClock.o : src/GPClock.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_GPClock.o -c src/GPClock.cpp -Iinclude -I../../include/
  
build/src_MGPCreator.o : src/MGPCreator.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_MGPCreator.o -c src/MGPCreator.cpp -Iinclude -I../../include/
  
build/src_MGPExecutor.o : src/MGPExecutor.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_MGPExecutor.o -c src/MGPExecutor.cpp -Iinclude -I../../include/
  
build/src_MGPKeyMatcher.o : src/MGPKeyMatcher.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_MGPKeyMatcher.o -c src/MGPKeyMatcher.cpp -Iinclude -I../../include/
  
build/src_MGPMachine.o : src/MGPMachine.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_MGPMachine.o -c src/MGPMachine.cpp -Iinclude -I../../include/
  
build/src_MGPPiecesFactory.o : src/MGPPiecesFactory.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_MGPPiecesFactory.o -c src/MGPPiecesFactory.cpp -Iinclude -I../../include/
  
build/src_MGPThread.o : src/MGPThread.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_MGPThread.o -c src/MGPThread.cpp -Iinclude -I../../include/
  
build/src_MGPThread_Linux.o : src/MGPThread_Linux.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_MGPThread_Linux.o -c src/MGPThread_Linux.cpp -Iinclude -I../../include/
  
build/src_MGPThreadPool.o : src/MGPThreadPool.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_MGPThreadPool.o -c src/MGPThreadPool.cpp -Iinclude -I../../include/
  

ALL_INCLUESexample.out=

example.out:  build/simple_main.o libmthpGP.so
	g++ -std=c++11   build/simple_main.o -O3 -g -fPIC   ./libmthpGP.so -ldl -lpthread -o example.out ${SELF_VARIABLES}
build/simple_main.o : simple_main.cpp libmthpGP.so  ${ALL_INCLUESexample.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/simple_main.o -c simple_main.cpp -Iinclude -I../../include/
  

clean:
	rm build/*.o
	rm libmthpGP.so
	rm example.out