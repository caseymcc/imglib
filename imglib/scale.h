#ifndef _imglib_scale_h_
#define _imglib_scale_h_

#include "imglib/image.h"
#include "imglib/interpolation.h"

namespace imglib
{

namespace details
{

bool scale(Format srcFormat, Depth srcDepth, size_t srcWidth, size_t srcHeight, size_t srcStride, const uint8_t *srcData, size_t srcDataSize,
    Format dstFormat, Depth dstDepth, size_t dstWidth, size_t dstHeight, size_t dstStride, uint8_t *dstData, size_t dstDataSize,
    Interpolation interpolation);

}//namespace details

template<typename _SrcImage, typename _DstImage>
bool scale(_SrcImage &srcImage, _DstImage &dstImage, Interpolation interpolation)
{
    return details::scale(format(srcImage), depth(srcImage), width(srcImage), height(srcImage), stride(srcImage), data(srcImage), dataSize(srcImage),
        format(dstImage), depth(dstImage), width(dstImage), height(dstImage), stride(dstImage), data(dstImage), dataSize(dstImage),
        interpolation);
}

}//namespace imglib
#endif//_imglib_scale_h_
