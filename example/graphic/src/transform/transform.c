#include "transform.h"
#include <stdio.h>
#include <assert.h>
#include "../third_party/libjpeg/jpeglib.h"
static inline void TrmultiPixels(TrPixels* p, int alpha)
{
    p->r = Tr_MULTI(p->r, alpha);
    p->g = Tr_MULTI(p->g, alpha);
    p->b = Tr_MULTI(p->b, alpha);
}

static inline void TrBlendPixels(TrPixels* src1, TrPixels* src2, TrPixels* dst, int alpha)
{
    dst->r = Tr_BLEND(src1->r, src2->r, alpha);
    dst->g = Tr_BLEND(src1->g, src2->g, alpha);
    dst->b = Tr_BLEND(src1->b, src2->b, alpha);
}
void TrMultyPixels(TrBmp* src, double factor)
{
    int i, size, alpha, temp;
    TrPixels* pixels;
    if (NULL==src) return;
    pixels = src->pixels;
    alpha = Tr_DOUBLE_TO_CHAR(factor);
    for (i=0; i<src->width*src->height; ++i)
    {
        TrmultiPixels(pixels+i, alpha);
    }
}

#define SCALE_UNIT 8
#define SCALE_ALPHA_MODE ((1<<SCALE_UNIT)-1)
TrBmp* TrScaleBmp(TrBmp* src, int targetWidth, int targetHeight)
{
    TrBmp* dst;
    int i,j;
    int src_i, src_j;
    int pos;
    int wScale, hScale;
    int wAlpha, hAlpha;
    TrPixels *src_, *dst_;
    assert(TrValidBmp(src));
    assert(targetWidth>0&&targetHeight>0);
    dst = TrAllocBmp(targetWidth, targetHeight);
    dst_ = dst->pixels;
    src_ = src->pixels;
    assert(NULL!=dst_);
    assert(NULL!=src_);
    wScale = (src->width << SCALE_UNIT) / targetWidth;
    hScale = (src->height << SCALE_UNIT) /targetHeight;
    for (i=0; i<targetHeight; ++i)
    {
        src_i = (i*hScale)>> SCALE_UNIT;
        for (j=0; j<targetWidth; ++j)
        {
            src_j = (j*wScale) >> SCALE_UNIT;
            *(dst_ + targetWidth*i+j) = *(src_+src->width*src_i + src_j);
        }
    }
    return dst;
}

TrBmp* TrMixturePixels(TrBmp* src1, TrBmp* src2, double alpha)
{
    assert(NULL!=src1);
    assert(NULL!=src2);
    int i,size, alpha_int;
    TrBmp* dst;
    unsigned char* src1_, src2_, dst_;
    size = src1->width*src1->height;
    assert(size ==src2->width*src2->height);
    dst = (TrBmp*)malloc(sizeof(TrBmp));
    assert(NULL!=dst);
    dst->pixels = malloc(size*sizeof(TrPixels));
    assert(NULL!=dst->pixels);
    alpha_int=Tr_DOUBLE_TO_CHAR(alpha);
    for (i=0; i < size; ++i)
    {
        TrBlendPixels(dst->pixels+i, src1->pixels+i, src2->pixels+i, alpha_int);
    }
    return dst;
}

void write_JPEG_file (const char * filename, JSAMPLE* image_buffer, int image_width, int image_height)
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * outfile;        /* target file */
  JSAMPROW row_pointer[1];    /* pointer to JSAMPLE row[s] */
  int row_stride;        /* physical row width in image buffer */
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  if ((outfile = fopen(filename, "wb")) == NULL) {
      return;
  }
  jpeg_stdio_dest(&cinfo, outfile);
  cinfo.image_width = image_width;     /* image width and height, in pixels */
  cinfo.image_height = image_height;
  cinfo.input_components = 3;        /* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB;     /* colorspace of input image */
  jpeg_set_defaults(&cinfo);

  jpeg_start_compress(&cinfo, TRUE);
  row_stride = image_width * 3;    /* JSAMPLEs per row in image_buffer */

  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }
  jpeg_finish_compress(&cinfo);
  fclose(outfile);

  jpeg_destroy_compress(&cinfo);

}
void TrWritePixels(TrBmp* src, const char* file)
{
    if (!TrValidBmp(src)) return;
    write_JPEG_file(file, (JSAMPLE*)(src->pixels), src->width, src->height);
    return;
}

TrBmp* TrLoadPixels(const char* file)
{
    TrBmp* output = NULL;
    TrPixels* cur;
    struct jpeg_error_mgr jerr;
    struct jpeg_decompress_struct cinfo;
    FILE* infile;
    JSAMPARRAY buffer;
    int row_stride;
    if ((infile = fopen(file, "rb")) == NULL) 
    {
        return NULL;
    }
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    (void) jpeg_read_header(&cinfo, TRUE);
    (void) jpeg_start_decompress(&cinfo);
    if (cinfo.output_components!=3)
    {
        printf("cinfo.output_components=%d \n", cinfo.output_components);
        return NULL;
    }
    output = (TrBmp*)malloc(sizeof(TrBmp));
    output->width = cinfo.output_width;
    output->height = cinfo.output_height;
    output->pixels = (TrPixels*)(malloc(output->width*output->height*sizeof(TrPixels)));
    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    cur = output->pixels;
    while (cinfo.output_scanline < cinfo.output_height)
    {
        int i; uchar* cbuffer;
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
        cbuffer = (uchar*)(buffer[0]);
        for (i=0;i<output->width; ++i)
        {
            cur->r = cbuffer[i*3];
            cur->g = cbuffer[i*3+1];
            cur->b = cbuffer[i*3+2];
            cur++;
        }
    }
    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return output;
}

TrBmp* TrColorSwap(TrBmp* src, PixelsSwapFunc func)
{
    int i;
    if (!TrValidBmp(src)) return NULL;
    TrBmp* dst = TrCopyAllocBmp(src);
    for (i=0; i<src->height*src->width; ++i)
    {
        func(dst->pixels+i);
    }
    return dst;
}


static void _Reverse(TrPixels* p)
{
    p->r = 255-p->r;
    p->g = 255-p->g;
    p->b = 255-p->b;
}
TrBmp* TrColorReverse(TrBmp* src)
{
    return TrColorSwap(src, _Reverse);
}
TrBmp* TrMixPicture(TrBmp** pictures, float* factor, int n, int width, int height)
{
    assert(NULL!=pictures && NULL!=factor && n>0);
    TrBmp* dst = TrAllocBmp(width, height);
    assert(TrValidBmp(dst));
    TrBmp** scalePics;
    /*Allocate and compute Scale pictures*/
    {
        scalePics = (TrBmp**)malloc(n*sizeof(TrBmp*));
        int i;
        for (i=0; i<n; ++i)
        {
            *(scalePics+i) = TrScaleBmp(*(pictures+i), width, height);
        }
    }
    /*Mixture*/
    {
        int i, x, y;
        for (x=0; x<width; ++x)
        {
            for(y=0; y<height; ++y)
            {
                int pos = y*width+x;
                float r=0, g=0, b=0;
                for (i=0; i<n;++i)
                {
                    r+= *(factor+i)*((scalePics[i]->pixels+pos)->r);
                    g+= *(factor+i)*((scalePics[i]->pixels+pos)->g);
                    b+= *(factor+i)*((scalePics[i]->pixels+pos)->b);
                }
                (dst->pixels+pos)->r = Tr_FLOAT_LIMIT_TO_CHAR(r);
                (dst->pixels+pos)->g = Tr_FLOAT_LIMIT_TO_CHAR(g);
                (dst->pixels+pos)->b = Tr_FLOAT_LIMIT_TO_CHAR(b);
            }
        }
    }

    /*Free temp pictures*/
    {
        int i;
        for (i=0; i<n; ++i)
        {
            TrFreeBmp(scalePics[i]);
        }
        free(scalePics);
    }
    return dst;
}

void TrDivide(TrBmp* src, TrBmp** dst)
{
    int w = src->width;
    int h = src->height;
    int size = w*h;
    int i;
    dst[0] = TrAllocBmp(w, h);
    dst[1] = TrAllocBmp(w, h);
    {
        TrPixels* srcP = src->pixels;
        TrPixels* dst0P = dst[0]->pixels;
        TrPixels* dst1P = dst[1]->pixels;
        for (i=0; i<size; ++i)
        {
            dst0P[i].r = 0;
            dst0P[i].g = 0;
            dst0P[i].b = 0;
            dst1P[i].r = 0;
            dst1P[i].g = 0;
            dst1P[i].b = 0;
            if (srcP[i].r > 100)
            {
                dst0P[i] = srcP[i];
            }
            else
            {
                dst1P[i] = srcP[i];
            }
        }
    }
}
