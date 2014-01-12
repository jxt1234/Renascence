include make.self
ALL_INCLUES=
all: GP_MAIN 
GP_MAIN:  build/table.o build/main.o
	g++  -O3 -o GP_MAIN  build/table.o build/main.o  ${SELF_VARIABLES}
build/table.o : src/table.cpp ${ALL_INCLUES}
	g++ -O3 -o build/table.o -c src/table.cpp -I${INCLUDE}
build/main.o : ./main.cpp ${ALL_INCLUES}
	g++ -O3 -o build/main.o -c ./main.cpp -I${INCLUDE}

clean:
	rm build/*.o
	rm GP_MAIN
