#ifndef _imglib_cuda_copy_h_
#define _imglib_cuda_copy_h_

#include "imglib/image.h"

#include <cassert>

namespace imglib
{
namespace cuda
{

bool copyImage(void *srcData, Location srcLocation, size_t srcStride, void *dstData, Location dstLocation, size_t dstStride, size_t width, size_t height);

template<typename _SrcImage, typename _DstImage>
bool copy(_SrcImage &srcImage, _DstImage &dstImage)
{
    if((location(srcImage) != Location::Cuda) && (location(dstImage) != Location::Cuda))
    {
        assert(false);//no cuda image here
        return false;
    }

    if((format(srcImage) != format(dstImage)) ||
        (depth(srcImage) != depth(dstImage)) ||
        (width(srcImage) != width(dstImage)) || 
        (height(srcImage) != height(dstImage)))
    {
        assert(false);//not implemented yet
        return false; //only supports same format, depth, width, height copy
    }

    size_t widthInBytes=width(srcImage)*sizeOfPixel(format(srcImage), depth(srcImage));

    return copyImage(data(srcImage), location(srcImage), stride(srcImage), data(dstImage), location(dstImage), stride(dstImage), widthInBytes, height(srcImage));
}

}//namespace cuda
}//namespace imglib

#endif //_imglib_cuda_copy_h_