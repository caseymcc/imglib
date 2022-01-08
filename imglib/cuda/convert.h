#ifndef _imglib_cuda_convert_h_
#define _imglib_cuda_convert_h_

#include "imglib/interpolation.h"
#include "copy.h"

#include <cuda_runtime.h>
#include <cassert>

namespace imglib
{
namespace cuda
{

cudaError_t convertNormMean_NearestNeighbor(void *input, Format inputFormat, size_t inputWidth, size_t inputHeight,
    float *output, Format outputFormat, size_t outputWidth, size_t outputHeight, 
    const float2 &range, const float3 &mean, const float3 &stdDev, cudaStream_t stream);

template<typename _SrcImage, typename _DstImage>
bool convertWithScale(_SrcImage &srcImage, _DstImage &dstImage, Interpolation interpolation=Interpolation::NearestNeighbor, cudaStream_t stream=(cudaStream_t)0)
{
    Format srcFormat=format(srcImage);
    Depth srcDepth=depth(srcImage);
    Format dstFormat=format(dstImage);
    Depth dstDepth=depth(dstImage);

    if(srcFormat == dstFormat)
    {
        if(dstDepth == Depth::Float)
        {
            if(interpolation == Interpolation::NearestNeighbor)
            {
                cudaError_t error=convertNormMean_NearestNeighbor((void *)data(srcImage), srcFormat, width(srcImage), height(srcImage), 
                    (float *)data(dstImage), dstFormat, width(dstImage), height(dstImage), 
                    make_float2(0.0f, 1.0f), make_float3(0.5f, 0.5f, 0.5f), make_float3(0.5f, 0.5f, 0.5f), stream);

                assert(error == cudaSuccess);
                return (error == cudaSuccess);
            }
        }
    }
    
    assert(false);//not implemented yet
    return false;
}


template<typename _SrcImage, typename _DstImage>
bool convert(_SrcImage &srcImage, _DstImage &dstImage, cudaStream_t stream=(cudaStream_t)0)
{
    if((location(srcImage) != Location::Cuda) || (location(dstImage) != Location::Cuda))
    {
        assert(false);//only cuda images supported
        return false;
    }

    if((width(srcImage) != width(dstImage)) || height(srcImage) != height(dstImage))
    {
        return convertWithScale(srcImage, dstImage, Interpolation::NearestNeighbor, stream);
    }

    Format srcFormat=format(srcImage);
    Depth srcDepth=depth(srcImage);
    Format dstFormat=format(dstImage);
    Depth dstDepth=depth(dstImage);

    if(srcDepth == dstDepth)
    {
        if(srcFormat == dstFormat)
        {
            return copy(srcImage, dstImage);
        }
    }
    else
    {
        if(dstDepth == Depth::Float)
        {
            return convertNormMean_NearestNeighbor((void *)data(srcImage), srcFormat, width(srcImage), height(srcImage), 
                    (float *)data(dstImage), dstFormat, width(dstImage), height(dstImage), 
                    make_float2(0.0f, 1.0f), make_float3(0.5f, 0.5f, 0.5f), make_float3(0.5f, 0.5f, 0.5f), stream);
        }
    }

    assert(false);//not implemented yet
    return false;
}

}//namespace cuda
}//namespace imglib

#endif//_imglib_cuda_convert_h_
