#ifndef _imglib_convert_h_
#define _imglib_convert_h_

#include "imglib_export.h"
#include "imglib/image.h"
#include "imglib/interpolation.h"
#include "imglib/copy.h"

#include <cassert>

namespace imglib
{

bool convertNormMean_NearestNeighbor(const void *input, Format inputFormat, size_t inputWidth, size_t inputHeight, size_t inputStride,
    float *output, Format outputFormat, size_t outputWidth, size_t outputHeight, size_t outputStride,
    const float *range/*[2]*/, const float *mean/*[3]*/, const float *stdDev/*[3]*/);

template<typename _SrcImage, typename _DstImage>
bool convertWithScale(_SrcImage &srcImage, _DstImage &dstImage, Interpolation interpolation=Interpolation::NearestNeighbor)
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
                float range[2]={0.0f, 1.0f};
                float mean[3]={0.5f, 0.5f, 0.5f};
                float stdDev[3]={0.5f, 0.5f, 0.5f};

                return convertNormMean_NearestNeighbor((void *)data(srcImage), srcFormat, width(srcImage), height(srcImage), stride(srcImage), 
                    (float *)data(dstImage), dstFormat, width(dstImage), height(dstImage), stride(dstImage), 
                    range, mean, stdDev);
            }
        }
    }
    
    assert(false);//not implemented yet
    return false;
}

template<typename _SrcImage, typename _DstImage>
bool convert(_SrcImage &srcImage, _DstImage &dstImage)
{
    if((location(srcImage) != Location::System) || (location(dstImage) != Location::System))
    {
        assert(false);//only cuda images supported
        return false;
    }

    if((width(srcImage) != width(dstImage)) || height(srcImage) != height(dstImage))
    {
        return convertWithScale(srcImage, dstImage, Interpolation::NearestNeighbor);
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
    else if(srcDepth == Depth::Bit8)
    {
        if(dstDepth == Depth::Float16)
        {

        }
        else if(dstDepth == Depth::Float)
        {
            float range[2]={0.0f, 1.0f};
            float mean[3]={0.5f, 0.5f, 0.5f};
            float stdDev[3]={0.5f, 0.5f, 0.5f};

            return convertNormMean_NearestNeighbor((void *)data(srcImage), srcFormat, width(srcImage), height(srcImage), stride(srcImage),
                    (float *)data(dstImage), dstFormat, width(dstImage), height(dstImage), stride(dstImage),
                    range, mean, stdDev);
        }
    }

    assert(false);//not implemented yet
    return false;
}

}//namespace imglib

#endif//_imglib_convert_h_
