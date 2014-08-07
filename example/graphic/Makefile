all:test.out libpics.so 

ALL_INCLUEStest.out= include/transform_format.h include/graphic.h include/core/gray.h include/core/cvt_color.h include/regress/f2c.h include/regress/ALMatrix.h include/regress/ALRefCount.h include/regress/blaswrap.h include/regress/ALFloatMatrix.h include/regress/clapack.h include/regress/ALUtils.h include/regress/graphic_regress.h include/debug.h include/transform_high.h include/transform.h include/package.h include/transform_math.h

test.out:  build/test_main.o libpics.so
	g++  -O3 -fPIC -o test.out  build/test_main.o  ./libpics.so ${SELF_VARIABLES}
build/test_main.o : test/main.c libpics.so  ${ALL_INCLUEStest.out}
	gcc -O3 -fPIC -o build/test_main.o -c test/main.c -Iinclude 

ALL_INCLUESlibpics.so= include/transform_format.h include/graphic.h include/core/gray.h include/core/cvt_color.h include/regress/f2c.h include/regress/ALMatrix.h include/regress/ALRefCount.h include/regress/blaswrap.h include/regress/ALFloatMatrix.h include/regress/clapack.h include/regress/ALUtils.h include/regress/graphic_regress.h include/debug.h include/transform_high.h include/transform.h include/package.h include/transform_math.h include/transform_format.h include/graphic.h include/core/gray.h include/core/cvt_color.h include/regress/f2c.h include/regress/ALMatrix.h include/regress/ALRefCount.h include/regress/blaswrap.h include/regress/ALFloatMatrix.h include/regress/clapack.h include/regress/ALUtils.h include/regress/graphic_regress.h include/debug.h include/transform_high.h include/transform.h include/package.h include/transform_math.h

libpics.so:  build/src_package_status_user.o build/src_package_package.o build/src_core_cvt_color.o build/src_regress_graphic_regress.o build/src_regress_ALMatrix.o build/src_regress_ALFloatMatrix.o build/src_core_graphic.o build/src_transform_transform_filter.o build/src_transform_transform.o build/src_transform_transform_math.o build/src_transform_transform_base.o build/src_transform_transform_high.o build/third_party_libjpeg_transupp.o build/third_party_libjpeg_jquant2.o build/third_party_libjpeg_jquant1.o build/third_party_libjpeg_jccolor.o build/third_party_libjpeg_jdapistd.o build/third_party_libjpeg_jdmerge.o build/third_party_libjpeg_jccoefct.o build/third_party_libjpeg_jdatadst.o build/third_party_libjpeg_jidctint.o build/third_party_libjpeg_jddctmgr.o build/third_party_libjpeg_jcomapi.o build/third_party_libjpeg_jdmaster.o build/third_party_libjpeg_jdmainct.o build/third_party_libjpeg_jcphuff.o build/third_party_libjpeg_jcparam.o build/third_party_libjpeg_jdphuff.o build/third_party_libjpeg_jdmarker.o build/third_party_libjpeg_jfdctint.o build/third_party_libjpeg_jdcoefct.o build/third_party_libjpeg_jutils.o build/third_party_libjpeg_jcmainct.o build/third_party_libjpeg_jdhuff.o build/third_party_libjpeg_jmemansi.o build/third_party_libjpeg_jcsample.o build/third_party_libjpeg_jcinit.o build/third_party_libjpeg_jdatasrc.o build/third_party_libjpeg_jcapimin.o build/third_party_libjpeg_jcmaster.o build/third_party_libjpeg_jerror.o build/third_party_libjpeg_jidctflt.o build/third_party_libjpeg_jidctfst.o build/third_party_libjpeg_jcapistd.o build/third_party_libjpeg_jdapimin.o build/third_party_libjpeg_jchuff.o build/third_party_libjpeg_jmemmgr.o build/third_party_libjpeg_jctrans.o build/third_party_libjpeg_jdpostct.o build/third_party_libjpeg_jcmarker.o build/third_party_libjpeg_jdcolor.o build/third_party_libjpeg_jdinput.o build/third_party_libjpeg_jidctred.o build/third_party_libjpeg_jcprepct.o build/third_party_libjpeg_jdtrans.o build/third_party_libjpeg_jcdctmgr.o build/third_party_libjpeg_jfdctfst.o build/third_party_libjpeg_jfdctflt.o build/third_party_libjpeg_jdsample.o 
	g++  -O3 -fPIC --share  -o libpics.so  build/src_package_status_user.o build/src_package_package.o build/src_core_cvt_color.o build/src_regress_graphic_regress.o build/src_regress_ALMatrix.o build/src_regress_ALFloatMatrix.o build/src_core_graphic.o build/src_transform_transform_filter.o build/src_transform_transform.o build/src_transform_transform_math.o build/src_transform_transform_base.o build/src_transform_transform_high.o build/third_party_libjpeg_transupp.o build/third_party_libjpeg_jquant2.o build/third_party_libjpeg_jquant1.o build/third_party_libjpeg_jccolor.o build/third_party_libjpeg_jdapistd.o build/third_party_libjpeg_jdmerge.o build/third_party_libjpeg_jccoefct.o build/third_party_libjpeg_jdatadst.o build/third_party_libjpeg_jidctint.o build/third_party_libjpeg_jddctmgr.o build/third_party_libjpeg_jcomapi.o build/third_party_libjpeg_jdmaster.o build/third_party_libjpeg_jdmainct.o build/third_party_libjpeg_jcphuff.o build/third_party_libjpeg_jcparam.o build/third_party_libjpeg_jdphuff.o build/third_party_libjpeg_jdmarker.o build/third_party_libjpeg_jfdctint.o build/third_party_libjpeg_jdcoefct.o build/third_party_libjpeg_jutils.o build/third_party_libjpeg_jcmainct.o build/third_party_libjpeg_jdhuff.o build/third_party_libjpeg_jmemansi.o build/third_party_libjpeg_jcsample.o build/third_party_libjpeg_jcinit.o build/third_party_libjpeg_jdatasrc.o build/third_party_libjpeg_jcapimin.o build/third_party_libjpeg_jcmaster.o build/third_party_libjpeg_jerror.o build/third_party_libjpeg_jidctflt.o build/third_party_libjpeg_jidctfst.o build/third_party_libjpeg_jcapistd.o build/third_party_libjpeg_jdapimin.o build/third_party_libjpeg_jchuff.o build/third_party_libjpeg_jmemmgr.o build/third_party_libjpeg_jctrans.o build/third_party_libjpeg_jdpostct.o build/third_party_libjpeg_jcmarker.o build/third_party_libjpeg_jdcolor.o build/third_party_libjpeg_jdinput.o build/third_party_libjpeg_jidctred.o build/third_party_libjpeg_jcprepct.o build/third_party_libjpeg_jdtrans.o build/third_party_libjpeg_jcdctmgr.o build/third_party_libjpeg_jfdctfst.o build/third_party_libjpeg_jfdctflt.o build/third_party_libjpeg_jdsample.o  ${SELF_VARIABLES}
build/src_package_status_user.o : src/package/status_user.cpp   ${ALL_INCLUESlibpics.so}
	g++ -O3 -fPIC -o build/src_package_status_user.o -c src/package/status_user.cpp -Iinclude 
build/src_package_package.o : src/package/package.cpp   ${ALL_INCLUESlibpics.so}
	g++ -O3 -fPIC -o build/src_package_package.o -c src/package/package.cpp -Iinclude 
build/src_core_cvt_color.o : src/core/cvt_color.cpp   ${ALL_INCLUESlibpics.so}
	g++ -O3 -fPIC -o build/src_core_cvt_color.o -c src/core/cvt_color.cpp -Iinclude 
build/src_regress_graphic_regress.o : src/regress/graphic_regress.cpp   ${ALL_INCLUESlibpics.so}
	g++ -O3 -fPIC -o build/src_regress_graphic_regress.o -c src/regress/graphic_regress.cpp -Iinclude 
build/src_regress_ALMatrix.o : src/regress/ALMatrix.cpp   ${ALL_INCLUESlibpics.so}
	g++ -O3 -fPIC -o build/src_regress_ALMatrix.o -c src/regress/ALMatrix.cpp -Iinclude 
build/src_regress_ALFloatMatrix.o : src/regress/ALFloatMatrix.cpp   ${ALL_INCLUESlibpics.so}
	g++ -O3 -fPIC -o build/src_regress_ALFloatMatrix.o -c src/regress/ALFloatMatrix.cpp -Iinclude 
build/src_core_graphic.o : src/core/graphic.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/src_core_graphic.o -c src/core/graphic.c -Iinclude 
build/src_transform_transform_filter.o : src/transform/transform_filter.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/src_transform_transform_filter.o -c src/transform/transform_filter.c -Iinclude 
build/src_transform_transform.o : src/transform/transform.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/src_transform_transform.o -c src/transform/transform.c -Iinclude 
build/src_transform_transform_math.o : src/transform/transform_math.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/src_transform_transform_math.o -c src/transform/transform_math.c -Iinclude 
build/src_transform_transform_base.o : src/transform/transform_base.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/src_transform_transform_base.o -c src/transform/transform_base.c -Iinclude 
build/src_transform_transform_high.o : src/transform/transform_high.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/src_transform_transform_high.o -c src/transform/transform_high.c -Iinclude 
build/third_party_libjpeg_transupp.o : third_party/libjpeg/transupp.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_transupp.o -c third_party/libjpeg/transupp.c -Iinclude 
build/third_party_libjpeg_jquant2.o : third_party/libjpeg/jquant2.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jquant2.o -c third_party/libjpeg/jquant2.c -Iinclude 
build/third_party_libjpeg_jquant1.o : third_party/libjpeg/jquant1.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jquant1.o -c third_party/libjpeg/jquant1.c -Iinclude 
build/third_party_libjpeg_jccolor.o : third_party/libjpeg/jccolor.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jccolor.o -c third_party/libjpeg/jccolor.c -Iinclude 
build/third_party_libjpeg_jdapistd.o : third_party/libjpeg/jdapistd.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jdapistd.o -c third_party/libjpeg/jdapistd.c -Iinclude 
build/third_party_libjpeg_jdmerge.o : third_party/libjpeg/jdmerge.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jdmerge.o -c third_party/libjpeg/jdmerge.c -Iinclude 
build/third_party_libjpeg_jccoefct.o : third_party/libjpeg/jccoefct.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jccoefct.o -c third_party/libjpeg/jccoefct.c -Iinclude 
build/third_party_libjpeg_jdatadst.o : third_party/libjpeg/jdatadst.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jdatadst.o -c third_party/libjpeg/jdatadst.c -Iinclude 
build/third_party_libjpeg_jidctint.o : third_party/libjpeg/jidctint.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jidctint.o -c third_party/libjpeg/jidctint.c -Iinclude 
build/third_party_libjpeg_jddctmgr.o : third_party/libjpeg/jddctmgr.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jddctmgr.o -c third_party/libjpeg/jddctmgr.c -Iinclude 
build/third_party_libjpeg_jcomapi.o : third_party/libjpeg/jcomapi.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jcomapi.o -c third_party/libjpeg/jcomapi.c -Iinclude 
build/third_party_libjpeg_jdmaster.o : third_party/libjpeg/jdmaster.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jdmaster.o -c third_party/libjpeg/jdmaster.c -Iinclude 
build/third_party_libjpeg_jdmainct.o : third_party/libjpeg/jdmainct.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jdmainct.o -c third_party/libjpeg/jdmainct.c -Iinclude 
build/third_party_libjpeg_jcphuff.o : third_party/libjpeg/jcphuff.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jcphuff.o -c third_party/libjpeg/jcphuff.c -Iinclude 
build/third_party_libjpeg_jcparam.o : third_party/libjpeg/jcparam.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jcparam.o -c third_party/libjpeg/jcparam.c -Iinclude 
build/third_party_libjpeg_jdphuff.o : third_party/libjpeg/jdphuff.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jdphuff.o -c third_party/libjpeg/jdphuff.c -Iinclude 
build/third_party_libjpeg_jdmarker.o : third_party/libjpeg/jdmarker.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jdmarker.o -c third_party/libjpeg/jdmarker.c -Iinclude 
build/third_party_libjpeg_jfdctint.o : third_party/libjpeg/jfdctint.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jfdctint.o -c third_party/libjpeg/jfdctint.c -Iinclude 
build/third_party_libjpeg_jdcoefct.o : third_party/libjpeg/jdcoefct.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jdcoefct.o -c third_party/libjpeg/jdcoefct.c -Iinclude 
build/third_party_libjpeg_jutils.o : third_party/libjpeg/jutils.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jutils.o -c third_party/libjpeg/jutils.c -Iinclude 
build/third_party_libjpeg_jcmainct.o : third_party/libjpeg/jcmainct.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jcmainct.o -c third_party/libjpeg/jcmainct.c -Iinclude 
build/third_party_libjpeg_jdhuff.o : third_party/libjpeg/jdhuff.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jdhuff.o -c third_party/libjpeg/jdhuff.c -Iinclude 
build/third_party_libjpeg_jmemansi.o : third_party/libjpeg/jmemansi.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jmemansi.o -c third_party/libjpeg/jmemansi.c -Iinclude 
build/third_party_libjpeg_jcsample.o : third_party/libjpeg/jcsample.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jcsample.o -c third_party/libjpeg/jcsample.c -Iinclude 
build/third_party_libjpeg_jcinit.o : third_party/libjpeg/jcinit.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jcinit.o -c third_party/libjpeg/jcinit.c -Iinclude 
build/third_party_libjpeg_jdatasrc.o : third_party/libjpeg/jdatasrc.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jdatasrc.o -c third_party/libjpeg/jdatasrc.c -Iinclude 
build/third_party_libjpeg_jcapimin.o : third_party/libjpeg/jcapimin.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jcapimin.o -c third_party/libjpeg/jcapimin.c -Iinclude 
build/third_party_libjpeg_jcmaster.o : third_party/libjpeg/jcmaster.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jcmaster.o -c third_party/libjpeg/jcmaster.c -Iinclude 
build/third_party_libjpeg_jerror.o : third_party/libjpeg/jerror.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jerror.o -c third_party/libjpeg/jerror.c -Iinclude 
build/third_party_libjpeg_jidctflt.o : third_party/libjpeg/jidctflt.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jidctflt.o -c third_party/libjpeg/jidctflt.c -Iinclude 
build/third_party_libjpeg_jidctfst.o : third_party/libjpeg/jidctfst.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jidctfst.o -c third_party/libjpeg/jidctfst.c -Iinclude 
build/third_party_libjpeg_jcapistd.o : third_party/libjpeg/jcapistd.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jcapistd.o -c third_party/libjpeg/jcapistd.c -Iinclude 
build/third_party_libjpeg_jdapimin.o : third_party/libjpeg/jdapimin.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jdapimin.o -c third_party/libjpeg/jdapimin.c -Iinclude 
build/third_party_libjpeg_jchuff.o : third_party/libjpeg/jchuff.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jchuff.o -c third_party/libjpeg/jchuff.c -Iinclude 
build/third_party_libjpeg_jmemmgr.o : third_party/libjpeg/jmemmgr.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jmemmgr.o -c third_party/libjpeg/jmemmgr.c -Iinclude 
build/third_party_libjpeg_jctrans.o : third_party/libjpeg/jctrans.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jctrans.o -c third_party/libjpeg/jctrans.c -Iinclude 
build/third_party_libjpeg_jdpostct.o : third_party/libjpeg/jdpostct.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jdpostct.o -c third_party/libjpeg/jdpostct.c -Iinclude 
build/third_party_libjpeg_jcmarker.o : third_party/libjpeg/jcmarker.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jcmarker.o -c third_party/libjpeg/jcmarker.c -Iinclude 
build/third_party_libjpeg_jdcolor.o : third_party/libjpeg/jdcolor.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jdcolor.o -c third_party/libjpeg/jdcolor.c -Iinclude 
build/third_party_libjpeg_jdinput.o : third_party/libjpeg/jdinput.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jdinput.o -c third_party/libjpeg/jdinput.c -Iinclude 
build/third_party_libjpeg_jidctred.o : third_party/libjpeg/jidctred.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jidctred.o -c third_party/libjpeg/jidctred.c -Iinclude 
build/third_party_libjpeg_jcprepct.o : third_party/libjpeg/jcprepct.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jcprepct.o -c third_party/libjpeg/jcprepct.c -Iinclude 
build/third_party_libjpeg_jdtrans.o : third_party/libjpeg/jdtrans.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jdtrans.o -c third_party/libjpeg/jdtrans.c -Iinclude 
build/third_party_libjpeg_jcdctmgr.o : third_party/libjpeg/jcdctmgr.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jcdctmgr.o -c third_party/libjpeg/jcdctmgr.c -Iinclude 
build/third_party_libjpeg_jfdctfst.o : third_party/libjpeg/jfdctfst.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jfdctfst.o -c third_party/libjpeg/jfdctfst.c -Iinclude 
build/third_party_libjpeg_jfdctflt.o : third_party/libjpeg/jfdctflt.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jfdctflt.o -c third_party/libjpeg/jfdctflt.c -Iinclude 
build/third_party_libjpeg_jdsample.o : third_party/libjpeg/jdsample.c   ${ALL_INCLUESlibpics.so}
	gcc -O3 -fPIC -o build/third_party_libjpeg_jdsample.o -c third_party/libjpeg/jdsample.c -Iinclude 

clean:
	rm build/*.o
	rm test.out
	rm libpics.so