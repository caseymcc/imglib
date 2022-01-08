#ifndef _imglib_crop_h_
#define _imglib_crop_h_

#include "imglib/image.h"
#include "imglib/vec.h"
#include "imglib/boundingBox.h"

namespace imglib
{

namespace details
{

bool crop(Format srcFormat, Depth srcDepth, size_t srcWidth, size_t srcHeight, size_t srcStride, const uint8_t *srcData, size_t srcDataSize,
    Format dstFormat, Depth dstDepth, size_t dstWidth, size_t dstHeight, size_t dstStride, uint8_t *dstData, size_t dstDataSize,
    int left, int right, int top, int bottom);

}//namespace details

template<typename _SrcImage, typename _DstImage, typename _BoundingBox>
bool crop(_SrcImage &srcImage, _DstImage &dstImage, _BoundingBox &boundingBox)
{
    return details::crop(format(srcImage), depth(srcImage), width(srcImage), height(srcImage), stride(srcImage), data(srcImage), dataSize(srcImage),
        format(dstImage), depth(dstImage), width(dstImage), height(dstImage), stride(dstImage), data(dstImage), dataSize(dstImage),
        bbox::left(boundingBox), bbox::right(boundingBox), bbox::top(boundingBox), bbox::bottom(boundingBox));
}

template<typename _SrcImage, typename _DstImage, typename _Point>
bool crop(_SrcImage &srcImage, _DstImage &dstImage, _Point &leftTop, _Point &rightBottom)
{
    return details::crop(format(srcImage), depth(srcImage), width(srcImage), height(srcImage), stride(srcImage), data(srcImage), dataSize(srcImage),
        format(dstImage), depth(dstImage), width(dstImage), height(dstImage), stride(dstImage), data(dstImage), dataSize(dstImage),
        vec::x(leftTop), vec::x(rightBottom), vec::y(leftTop), vec::y(rightBottom));
}

template<typename _SrcImage, typename _DstImage, typename _Point>
bool crop(_SrcImage &srcImage, _DstImage &dstImage, int left, int right, int top, int bottom)
{
    return details::crop(format(srcImage), depth(srcImage), width(srcImage), height(srcImage), stride(srcImage), data(srcImage), dataSize(srcImage),
        format(dstImage), depth(dstImage), width(dstImage), height(dstImage), stride(dstImage), data(dstImage), dataSize(dstImage),
        left, right, top, bottom);
}

}//namespace imglib

#endif//_imglib_crop_h_
