#include "user/GPStream.h"

class GPFileStream:public GPStream
{
public:
    GPFileStream(const char* filename);
    virtual ~GPFileStream();
};

class GPFileWStream:public GPWStream
{
public:
    GPFileWStream(const char* filename);
    virtual ~GPFileWStream();
};