all:libGPJni.so 

ALL_INCLUESlibGPJni.so=

libGPJni.so:  build/_swig_renascence_basic_wrap.o 
	g++ -std=c++11   build/_swig_renascence_basic_wrap.o -O3 -g -fPIC  --shared   ../libGP.a -o libGPJni.so ${SELF_VARIABLES}
build/_swig_renascence_basic_wrap.o : ../swig/renascence_basic_wrap.cxx   ${ALL_INCLUESlibGPJni.so}
	g++ -std=c++11 -O3 -g -fPIC -o build/_swig_renascence_basic_wrap.o -c ../swig/renascence_basic_wrap.cxx -I../include -I/Library/Java/JavaVirtualMachines/jdk1.8.0_40.jdk/Contents/Home/include/darwin/ -I/Library/Java/JavaVirtualMachines/jdk1.8.0_40.jdk/Contents/Home/include/
  

clean:
	rm build/*.o
	rm libGPJni.so