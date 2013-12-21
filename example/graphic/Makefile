include make.self
ALL_INCLUES= include/transform_format.h include/graphic.h include/core/cvt_color.h include/regress/f2c.h include/regress/blaswrap.h include/regress/clapack.h include/regress/graphic_regress.h include/debug.h include/transform_high.h include/transform.h include/package.h include/transform_math.h
all: GP_MAIN libpics.so 
GP_MAIN:  build/package_status_user.o build/package_package.o build/core_cvt_color.o build/core_graphic.o build/regress_graphic_regress.o build/transform_transform_filter.o build/transform_transform.o build/transform_transform_math.o build/transform_transform_base.o build/transform_transform_high.o build/main.o
	g++  -O3 -o GP_MAIN  build/package_status_user.o build/package_package.o build/core_cvt_color.o build/core_graphic.o build/regress_graphic_regress.o build/transform_transform_filter.o build/transform_transform.o build/transform_transform_math.o build/transform_transform_base.o build/transform_transform_high.o build/main.o  ${SELF_VARIABLES}
libpics.so:  build/package_status_user.o build/package_package.o build/core_cvt_color.o build/core_graphic.o build/regress_graphic_regress.o build/transform_transform_filter.o build/transform_transform.o build/transform_transform_math.o build/transform_transform_base.o build/transform_transform_high.o build/main.o
	g++  -O3 --share  -o libpics.so  build/package_status_user.o build/package_package.o build/core_cvt_color.o build/core_graphic.o build/regress_graphic_regress.o build/transform_transform_filter.o build/transform_transform.o build/transform_transform_math.o build/transform_transform_base.o build/transform_transform_high.o build/main.o  ${SELF_VARIABLES}
build/package_status_user.o : src/package/status_user.cpp ${ALL_INCLUES}
	g++ -O3 -o build/package_status_user.o -c src/package/status_user.cpp -Iinclude 
build/package_package.o : src/package/package.cpp ${ALL_INCLUES}
	g++ -O3 -o build/package_package.o -c src/package/package.cpp -Iinclude 
build/core_cvt_color.o : src/core/cvt_color.cpp ${ALL_INCLUES}
	g++ -O3 -o build/core_cvt_color.o -c src/core/cvt_color.cpp -Iinclude 
build/core_graphic.o : src/core/graphic.c ${ALL_INCLUES}
	gcc -O3 -o build/core_graphic.o -c src/core/graphic.c -Iinclude 
build/regress_graphic_regress.o : src/regress/graphic_regress.c ${ALL_INCLUES}
	gcc -O3 -o build/regress_graphic_regress.o -c src/regress/graphic_regress.c -Iinclude 
build/transform_transform_filter.o : src/transform/transform_filter.c ${ALL_INCLUES}
	gcc -O3 -o build/transform_transform_filter.o -c src/transform/transform_filter.c -Iinclude 
build/transform_transform.o : src/transform/transform.c ${ALL_INCLUES}
	gcc -O3 -o build/transform_transform.o -c src/transform/transform.c -Iinclude 
build/transform_transform_math.o : src/transform/transform_math.c ${ALL_INCLUES}
	gcc -O3 -o build/transform_transform_math.o -c src/transform/transform_math.c -Iinclude 
build/transform_transform_base.o : src/transform/transform_base.c ${ALL_INCLUES}
	gcc -O3 -o build/transform_transform_base.o -c src/transform/transform_base.c -Iinclude 
build/transform_transform_high.o : src/transform/transform_high.c ${ALL_INCLUES}
	gcc -O3 -o build/transform_transform_high.o -c src/transform/transform_high.c -Iinclude 
build/main.o : ./main.c ${ALL_INCLUES}
	gcc -O3 -o build/main.o -c ./main.c -Iinclude 

clean:
	rm build/*.o
	rm GP_MAIN
	rm libpics.so