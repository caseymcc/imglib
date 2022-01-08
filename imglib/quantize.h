#ifndef _imglib_quantize_h_
#define _imglib_quantize_h_

#include "imglib_export.h"
#include "imglib/image.h"
#include "imglib/interpolation.h"

#include <cassert>

namespace imglib
{

namespace details
{

bool quantize(Format inputFormat, size_t inputWidth, size_t inputHeight, size_t inputStride, const float *input,
    Format outputFormat, Depth outputDepth, size_t outputWidth, size_t outputHeight, size_t outputStride, uint8_t *output);

}//namespace details

template<typename _SrcImage, typename _DstImage>
bool quantize(_SrcImage &srcImage, _DstImage &dstImage)
{
    return details::quantize(format(srcImage), width(srcImage), height(srcImage), stride(srcImage), (float *)data(srcImage),
        format(dstImage), depth(dstImage), width(dstImage), height(dstImage), stride(dstImage), data(dstImage));
}

}//namespace imglib

#endif//_imglib_quantize_h_
