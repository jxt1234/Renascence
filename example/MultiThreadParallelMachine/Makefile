all:libmthpGP.so example.out 

ALL_INCLUESlibmthpGP.so= include//MGPCreator.h include//MGPExecutor.h include//MGPMachine.h include//MGPPiecesFactory.h

libmthpGP.so:  build/src_MGPExecutor.o build/src_MGPMachine.o build/src_MGPPiecesFactory.o 
	g++ -std=c++11   build/src_MGPExecutor.o build/src_MGPMachine.o build/src_MGPPiecesFactory.o -O3 -g -fPIC  --shared  -lpthread -ldl -o libmthpGP.so ${SELF_VARIABLES}
build/src_MGPExecutor.o : src/MGPExecutor.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_MGPExecutor.o -c src/MGPExecutor.cpp -Iinclude -I../../include/
  
build/src_MGPMachine.o : src/MGPMachine.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_MGPMachine.o -c src/MGPMachine.cpp -Iinclude -I../../include/
  
build/src_MGPPiecesFactory.o : src/MGPPiecesFactory.cpp   ${ALL_INCLUESlibmthpGP.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/src_MGPPiecesFactory.o -c src/MGPPiecesFactory.cpp -Iinclude -I../../include/
  

ALL_INCLUESexample.out=

example.out:  build/simple_main.o libmthpGP.so
	g++ -std=c++11   build/simple_main.o -O3 -g -fPIC   ./libmthpGP.so -ldl -lpthread -o example.out ${SELF_VARIABLES}
build/simple_main.o : simple_main.cpp libmthpGP.so  ${ALL_INCLUESexample.out}
	g++ -std=c++11 -O3 -g -fPIC -o build/simple_main.o -c simple_main.cpp -Iinclude -I../../include/
  

clean:
	rm build/*.o
	rm libmthpGP.so
	rm example.out