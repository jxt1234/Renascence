#include "TrJpegSourceMgr.h"

/////////////////////////////////////////////////////////////////////
static void sk_init_source(j_decompress_ptr cinfo) {
    skjpeg_source_mgr*  src = (skjpeg_source_mgr*)cinfo->src;
    src->next_input_byte = (const JOCTET*)src->fBuffer;
    src->bytes_in_buffer = 0;
}

static boolean sk_fill_input_buffer(j_decompress_ptr cinfo) {
    skjpeg_source_mgr* src = (skjpeg_source_mgr*)cinfo->src;
    size_t bytes = src->fStream->read(src->fBuffer, skjpeg_source_mgr::kBufferSize);
    // note that JPEG is happy with less than the full read,
    // as long as the result is non-zero
    if (bytes == 0) {
        return FALSE;
    }

    src->next_input_byte = (const JOCTET*)src->fBuffer;
    src->bytes_in_buffer = bytes;
    return TRUE;
}

static void sk_skip_input_data(j_decompress_ptr cinfo, long num_bytes) {
    skjpeg_source_mgr*  src = (skjpeg_source_mgr*)cinfo->src;

    if (num_bytes > (long)src->bytes_in_buffer) {
        size_t bytesToSkip = num_bytes - src->bytes_in_buffer;
        while (bytesToSkip > 0) {
            size_t bytes = src->fStream->read(NULL, bytesToSkip);
            if (bytes <= 0 || bytes > bytesToSkip) {
//              SkDebugf("xxxxxxxxxxxxxx failure to skip request %d returned %d\n", bytesToSkip, bytes);
                cinfo->err->error_exit((j_common_ptr)cinfo);
                return;
            }
            bytesToSkip -= bytes;
        }
        src->next_input_byte = (const JOCTET*)src->fBuffer;
        src->bytes_in_buffer = 0;
    } else {
        src->next_input_byte += num_bytes;
        src->bytes_in_buffer -= num_bytes;
    }
}

static void sk_term_source(j_decompress_ptr /*cinfo*/) {}


///////////////////////////////////////////////////////////////////////////////

skjpeg_source_mgr::skjpeg_source_mgr(GPStream* stream)
    : fStream(stream) {

    init_source = sk_init_source;
    fill_input_buffer = sk_fill_input_buffer;
    skip_input_data = sk_skip_input_data;
    resync_to_restart = jpeg_resync_to_restart;
    term_source = sk_term_source;
}

skjpeg_source_mgr::~skjpeg_source_mgr() {
}

///////////////////////////////////////////////////////////////////////////////

static void sk_init_destination(j_compress_ptr cinfo) {
    skjpeg_destination_mgr* dest = (skjpeg_destination_mgr*)cinfo->dest;

    dest->next_output_byte = dest->fBuffer;
    dest->free_in_buffer = skjpeg_destination_mgr::kBufferSize;
}

static boolean sk_empty_output_buffer(j_compress_ptr cinfo) {
    skjpeg_destination_mgr* dest = (skjpeg_destination_mgr*)cinfo->dest;

//  if (!dest->fStream->write(dest->fBuffer, skjpeg_destination_mgr::kBufferSize - dest->free_in_buffer))
    if (!dest->fStream->write(dest->fBuffer,
            skjpeg_destination_mgr::kBufferSize)) {
        ERREXIT(cinfo, JERR_FILE_WRITE);
        return false;
    }

    dest->next_output_byte = dest->fBuffer;
    dest->free_in_buffer = skjpeg_destination_mgr::kBufferSize;
    return TRUE;
}

static void sk_term_destination (j_compress_ptr cinfo) {
    skjpeg_destination_mgr* dest = (skjpeg_destination_mgr*)cinfo->dest;

    size_t size = skjpeg_destination_mgr::kBufferSize - dest->free_in_buffer;
    if (size > 0) {
        if (!dest->fStream->write(dest->fBuffer, size)) {
            ERREXIT(cinfo, JERR_FILE_WRITE);
            return;
        }
    }
    dest->fStream->flush();
}

skjpeg_destination_mgr::skjpeg_destination_mgr(GPWStream* stream)
        : fStream(stream) {
    this->init_destination = sk_init_destination;
    this->empty_output_buffer = sk_empty_output_buffer;
    this->term_destination = sk_term_destination;
}

void skjpeg_error_exit(j_common_ptr cinfo) {
    skjpeg_error_mgr* error = (skjpeg_error_mgr*)cinfo->err;

    (*error->output_message) (cinfo);

    /* Let the memory manager delete any temp files before we die */
    jpeg_destroy(cinfo);

    longjmp(error->fJmpBuf, -1);
}
