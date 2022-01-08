#include "imglib/math.h"

namespace imglib
{

namespace details
{

uint8_t mean(Format format, Depth depth, size_t width, size_t height, size_t stride, 
    const uint8_t *data, size_t dataSize)
{
    if(format != Format::GreyScale)
        return 0.0f;

    if(depth != Depth::Bit8)
        return 0.0f;

    int32_t accumulator=0;

    for(size_t i=0; i<dataSize; ++i)
    {
        accumulator+=data[i];
    }

    return (uint8_t)(accumulator/dataSize);
}

}//namespace details
}//namespace imglib