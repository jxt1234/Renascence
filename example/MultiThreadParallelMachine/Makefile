all:libmthpGP.so example.out 

ALL_INCLUESlibmthpGP.so= include//AutoStorage.h include//GPClock.h include//MGPExecutor.h include//MGPMachine.h include//MGPThread.h include//MGPThreadPool.h include//MGPUtils.h

libmthpGP.so:  build/src_GPClock_cpp.o build/src_MGPExecutor_cpp.o build/src_MGPKeyMatcher_cpp.o build/src_MGPMachine_cpp.o build/src_MGPThread_cpp.o build/src_MGPThread_Linux_cpp.o build/src_MGPThreadPool_cpp.o 
	g++ -std=c++11   build/src_GPClock_cpp.o build/src_MGPExecutor_cpp.o build/src_MGPKeyMatcher_cpp.o build/src_MGPMachine_cpp.o build/src_MGPThread_cpp.o build/src_MGPThread_Linux_cpp.o build/src_MGPThreadPool_cpp.o -O3 -g -fPIC  --shared  ../../libGP.a -lpthread -ldl -o libmthpGP.so ${SELF_VARIABLES}
build/src_GPClock_cpp.o : src/GPClock.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_GPClock_cpp.o -c src/GPClock.cpp -Iinclude -I../../include/
  
build/src_MGPExecutor_cpp.o : src/MGPExecutor.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_MGPExecutor_cpp.o -c src/MGPExecutor.cpp -Iinclude -I../../include/
  
build/src_MGPKeyMatcher_cpp.o : src/MGPKeyMatcher.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_MGPKeyMatcher_cpp.o -c src/MGPKeyMatcher.cpp -Iinclude -I../../include/
  
build/src_MGPMachine_cpp.o : src/MGPMachine.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_MGPMachine_cpp.o -c src/MGPMachine.cpp -Iinclude -I../../include/
  
build/src_MGPThread_cpp.o : src/MGPThread.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_MGPThread_cpp.o -c src/MGPThread.cpp -Iinclude -I../../include/
  
build/src_MGPThread_Linux_cpp.o : src/MGPThread_Linux.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_MGPThread_Linux_cpp.o -c src/MGPThread_Linux.cpp -Iinclude -I../../include/
  
build/src_MGPThreadPool_cpp.o : src/MGPThreadPool.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_MGPThreadPool_cpp.o -c src/MGPThreadPool.cpp -Iinclude -I../../include/
  

ALL_INCLUESexample.out=

example.out:  build/simple_main_cpp.o libmthpGP.so
	g++ -std=c++11   build/simple_main_cpp.o -O3 -g -fPIC   ./libmthpGP.so -ldl -lpthread -o example.out ${SELF_VARIABLES}
build/simple_main_cpp.o : simple_main.cpp libmthpGP.so  ${ALL_INCLUESexample.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/simple_main_cpp.o -c simple_main.cpp -Iinclude -I../../include/
  

clean:
	rm build/*.o
	rm libmthpGP.so
	rm example.out