#include "core/GPStreamFactory.h"

class GPFileStream:public GPStreamWrap
{
public:
    GPFileStream(const char* filename);
    virtual ~GPFileStream();
};

class GPFileWStream:public GPWStreamWrap
{
public:
    GPFileWStream(const char* filename);
    virtual ~GPFileWStream();
};