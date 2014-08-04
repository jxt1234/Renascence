#include <stdio.h>
#include <stdlib.h>
#include "regress/f2c.h"
#include "regress/clapack.h"
#include "regress/graphic_regress.h"

void test_TrRegressMatrix(const char* srcP, const char* dstP)
{
	char* result;
	TrRegreeMode* mode = TrRegreeModeAlloc(3);
	TrBmp* src = TrLoadPixels(srcP);
	TrBmp* dst = TrLoadPixels(dstP);
	TrFilterMatrix* matrix;
	mode->mode[2] = 0;
	mode->mode[4] = 0;
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
	test_satu("input.jpg", "input_satu.jpg", 1.5);
}
