#include "imglib/fastcv/scale.h"

#include <fastcv.h>

namespace imglib
{

namespace fastcv
{

bool scale(Format srcFormat, size_t srcWidth, size_t srcHeight, size_t srcStride, const uint8_t *srcData, size_t srcDataSize,
    Format dstFormat, size_t dstWidth, size_t dstHeight, size_t dstStride, uint8_t *dstData, size_t dstDataSize,
    Interpolation interpolation)
{
    if(interpolation == Interpolation::NearestNeighbor)
    {
        fcvScaleDownNNu8(srcData, srcStride, srcHeight, srcStride, dstData, dstStride, dstHeight, dstStride);
        return true;
    }
    else if(interpolation == Interpolation::BiLinear)
    {
        fcvScaleDownBLu8(srcData, srcStride, srcHeight, srcStride, dstData, dstStride, dstHeight, dstStride);
        return true;
    }

    return false;
}

}//namespace fastcv
}//namespace imglib