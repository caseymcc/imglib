#ifndef _imglib_fastcv_scale_h_
#define _imglib_fastcv_scale_h_

#include "imglib/image.h"
#include "imglib/interpolation.h"

namespace imglib
{

namespace fastcv
{

bool scale(Format srcFormat, size_t srcWidth, size_t srcHeight, size_t srcStride, const uint8_t *srcData, size_t srcDataSize,
    Format dstFormat, size_t dstWidth, size_t dstHeight, size_t dstStride, uint8_t *dstData, size_t dstDataSize,
    Interpolation interpolation);

}//namespace fastcv
}//namespace imglib
#endif//_imglib_fastcv_scale_h_