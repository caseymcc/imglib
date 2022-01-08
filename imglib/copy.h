#ifndef _imglib_copy_h_
#define _imglib_copy_h_

#ifdef IMGLIB_CUDA
#include "imglib/cuda/copy.h"
#endif//IMGLIB_CUDA

#include <memory>
#include <cstring>
#include <cassert>

#include "imglib_export.h"


namespace imglib
{

template<typename _SrcImage, typename _DstImage>
bool copy(_SrcImage &srcImage, _DstImage &dstImage)
{
    if((location(srcImage) != Location::System) || (location(dstImage) != Location::System))
    {
#ifdef IMGLIB_CUDA    
        if((location(srcImage) == Location::Cuda) || (location(dstImage) == Location::Cuda))
            return cuda::copy(srcImage, dstImage);
        assert(false);//not implemented yet
        return false;
#endif//IMGLIB_CUDA
    }

    return copyTo(srcImage, dstImage, 0, 0);
}

template<typename _SrcImage, typename _DstImage>
bool copyTo(_SrcImage &srcImage, _DstImage &dstImage, size_t dstX, size_t dstY)
{
    if((location(srcImage) != Location::System) || (location(dstImage) != Location::System))
    {
        assert(false);//not implemented yet
        return false;
    }

    if(format(srcImage) != format(dstImage))
    {
        assert(false);//not implemented yet
        return false;
    }

    if(depth(srcImage) != depth(dstImage))
    {
        assert(false);//not implemented yet
        return false;
    }

    //raw copy
    size_t copyWidth=width(srcImage);
    size_t copyHeight=height(srcImage);
    
    if(copyWidth+dstX>width(dstImage))
        copyWidth=width(dstImage)-dstX;
    copyWidth*=sizeOfPixel(format(srcImage), depth(srcImage));

    if(copyHeight+dstY>height(dstImage))
        copyHeight=height(dstImage)-dstY;

    size_t srcStride=stride(srcImage);
    uint8_t *srcData=data(srcImage);
        
    size_t dstStride=stride(dstImage);
    uint8_t *dstData=data(dstImage)+(dstStride*dstY)+(sizeOfPixel(format(dstImage), depth(dstImage))*dstX);
    
    for(size_t y=0; y<copyHeight; ++y)
    {
        memcpy(dstData, srcData, copyWidth);

        srcData+=srcStride;
        dstData+=dstStride;
    }

    return true;
}

}//namespace imglib

#endif //_imglib_copy_h_