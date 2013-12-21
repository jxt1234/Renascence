#include <stdio.h>
#include <stdlib.h>
#include "regress/f2c.h"
#include "regress/clapack.h"
#include "regress/graphic_regress.h"

void test_lapack()
{
	int i;
	integer M = 5;
	integer N = 2;
	integer P = 0;
	real A[]={1.0,2.0,3.0,4.0,5.0, 1.0, 1.0, 1.0,1.0,1.0};
	real B[]={};
	integer LDB = 1;
	real C[] = {16.1, 32.0, 48.0, 64.0, 80.0};
	real D[]={0};
	real X[2];
	integer LWORK = M+N+P;
	real* WORK = (real*)malloc(sizeof(real)*LWORK);
	integer INFO;
	sgglse_(&M, &N, &P, A, &M, B, &LDB, C, D, X, WORK, &LWORK, &INFO);
	for (i=0; i<N; ++i)
	{
		printf("X[%d]=%f\n", i, X[i]);
	}
	free(WORK);
}

void test_TrRegressMatrix(const char* srcP, const char* dstP)
{
	char* result;
	TrRegreeMode* mode = TrRegreeModeAlloc(3);
	TrBmp* src = TrLoadPixels(srcP);
	TrBmp* dst = TrLoadPixels(dstP);
	TrFilterMatrix* matrix;
	matrix = TrRegressMatrix(src, dst, mode);
	result = TrFilterMatrixPrint(matrix);
	printf("%s\n", result);
	free(result);

	TrBmp* fact = TrFilterMatrixTransform(src, matrix);
	printf("\nerror = %f\n", TrCompareBmp(dst, fact));

	TrWritePixels(fact, "test_regress.jpg");
	TrFreeBmp(fact);
	TrFreeBmp(src);
	TrFreeBmp(dst);
	TrFilterMatrixFree(matrix);
	TrRegreeModeFree(mode);
}

void test_TrRegressMixFactor(const char* src1, const char* src2, const char* dstP)
{
	float parameters[2]={0,0};
	TrBmp* src[2];
	src[0] = TrLoadPixels(src1);
	src[1] = TrLoadPixels(src2);
	TrBmp* dst = TrLoadPixels(dstP);
	TrRegressMixFactor(dst, src, 2, parameters);
	printf("\np1= %f, p2=%f\n", parameters[0], parameters[1]);

	TrBmp* fact = TrMixPicture(src, parameters, 2, dst->width, dst->height);
	TrWritePixels(fact, "test_regress_mix.jpg");
	printf("\nerror = %f\n", TrCompareBmp(fact, dst));

	TrFreeBmp(fact);
	TrFreeBmp(src[0]);
	TrFreeBmp(src[1]);
	TrFreeBmp(dst);
}


void test_satu(const char* input, const char* output, float value)
{
	TrBmp* src = TrLoadPixels(input);
	TrBmp* dst = TrSaturation(src, value);
	TrFreeBmp(src);
	TrWritePixels(dst, output);
	TrFreeBmp(dst);
}


int main()
{
	//test_lapack();
	test_TrRegressMatrix("test.jpg", "test_relif.jpg");
	test_TrRegressMixFactor("test.jpg", "test_relif.jpg", "test_mix.jpg");
	test_satu("input.jpg", "input_satu.jpg", 1.5);
}
