#include <stdio.h>



#include "transform.h"
#include "transform_high.h"

void rgswap(TrPixels* pixel)
{
	uchar temp;
	temp = pixel->r;
	pixel->r = pixel->g;
	pixel->g = temp;
}

void r_2(TrPixels* pixel)
{
	pixel->r/=2;
}
void r_g_b_equal_r(TrPixels* pixel)
{
	pixel->g= pixel->r;
	pixel->b = pixel->r;
}

void grayer(TrPixels* pixel)
{
	float gray = 0;
	gray = 0.31*pixel->r + 0.44*pixel->g + 0.25*pixel->b;
	uchar _gray = Tr_FLOAT_LIMIT_TO_CHAR(gray);
	pixel->r = _gray;
	pixel->g = _gray;
	pixel->b = _gray;
}



float matrix[9] = {0.0,0.0,0.0,-1.0,1.0,0.0,0.0,0.0,0.0};
//float matrix[9] = {0.1,0.1,0.1,0.1,0.2,0.1,0.1,0.1,0.1};

int main()
{
	TrBmp* src = TrLoadPixels("../../input.jpg");
	TrBmp* dst;
	//TrBmp* dst = TrScaleBmp(src, src->width*2, src->height*2);
	dst=TrColorSwap(src, grayer);
	TrWritePixels(dst, "test_colorswap.jpg");
	TrFreeBmp(dst);

	TrBmp* dst2;
	dst2 = TrNormalBlur(src);
	TrWritePixels(dst2, "test_blur.jpg");
	TrFreeBmp(dst2);

	TrBmp* dst3 = TrSimpleSharp(src, 100,0);
	TrWritePixels(dst3, "test_sharp.jpg");
	TrFreeBmp(dst3);
	
	TrBmp* dst4 = TrColorReverse(src);
	TrWritePixels(dst4, "test_reverse.jpg");
	TrFreeBmp(dst4);

	dst4 = TrFilterRelief(src);
	TrWritePixels(dst4, "test_relif.jpg");
	TrFreeBmp(dst4);

	TrBmp* dst5 = TrFilterRainbow(src);
	TrWritePixels(dst5, "test_rainbow.jpg");
	TrFreeBmp(dst5);

	TrFilterMatrix* m = TrFilterMatrixCopyAlloc(matrix, 3);
	m->offset = 128;
	dst5 = TrFilterMatrixTransform(src, m);
	TrWritePixels(dst5, "test_filter_matrix.jpg");
	TrFilterMatrixFree(m);

	TrBmp* pics[] = {src, dst5};
	float factor[] = {0.3, 0.7};
	TrBmp* dst6 = TrMixPicture(pics, factor, 2, src->width, src->height);
	TrWritePixels(dst6, "test_mix.jpg");

	printf("\nvalue=%f\n",TrCompareBmp(dst5, dst6));

	TrBmp* dst7 = TrSaturation(src, 2.0);
	TrWritePixels(dst7, "test_saturation.jpg");


	TrFreeBmp(dst7);
	TrFreeBmp(dst5);
	TrFreeBmp(dst6);
	TrFreeBmp(src);
	return 0;
}
