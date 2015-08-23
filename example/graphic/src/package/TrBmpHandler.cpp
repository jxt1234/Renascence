extern "C"
{
#include "transform.h"
}
#include "TrJpegSourceMgr.h"
TrBmp* TrLoadPixelsFromStream(GPStream* stream)
{
    jpeg_decompress_struct cinfo;
    skjpeg_source_mgr source_mgr(stream);
    skjpeg_error_mgr errorManager;
    cinfo.err = jpeg_std_error(&errorManager);
    errorManager.error_exit = skjpeg_error_exit;
    jpeg_create_decompress(&cinfo);
    cinfo.src = &source_mgr;
    TrBmp* output = NULL;
    TrPixels* cur;
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
    int row_stride = cinfo.output_width * cinfo.output_components;
    JSAMPARRAY buffer;
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
    return output;
}
void TrWritePixelsToStream(TrBmp* src, struct GPWStream* wstream)
{
    if (!TrValidBmp(src)) return;
    int image_width = src->width;
    int image_height = src->height;
    JSAMPLE* image_buffer = (JSAMPLE*)src->pixels;
    skjpeg_destination_mgr dest_mgr(wstream);
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE * outfile;        /* target file */
    JSAMPROW row_pointer[1];    /* pointer to JSAMPLE row[s] */
    int row_stride;        /* physical row width in image buffer */
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    cinfo.dest = &dest_mgr;
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
    jpeg_destroy_compress(&cinfo);
}
