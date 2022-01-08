#include "imglib/scale.h"

#ifdef IMGLIB_USE_FASTCV
#include "imglib/fastcv/scale.h"
#endif//IMGLIB_USE_FASTCV

#include <cassert>

namespace imglib
{

template<typename _SrcType, typename _DstType, size_t channels>
bool scale_NearestNeighbor_Plane(size_t srcWidth, size_t srcHeight, size_t srcStride, const _SrcType *srcData,
    size_t dstWidth, const size_t dstHeight, size_t dstStride, _DstType *dstData,
    const float *scale)
{
    const int dstPlaneStride=dstWidth*dstHeight;
    _DstType *channel[channels];
    
    for(size_t i=0; i<channels; ++i)
        channel[i]=dstData+(i*dstPlaneStride);

    for(size_t y=0; y<dstHeight; ++y)
    {
        const int dy=(int)((float)y*scale[1])*srcWidth;

        for(size_t x=0; x<dstWidth; ++x)
        {
            const int dx=((float)x*scale[0]);
            const _SrcType *pixel=&srcData[dy+dx];

            for(size_t i=0; i<channels; ++i)
            {
                (*channel[i])=(_DstType)pixel[i];
                channel[i]++;
            }
        }
    }

    return true;
}

template<typename _SrcType, typename _DstType, size_t channels>
bool scale_NearestNeighbor_Packed(size_t srcWidth, size_t srcHeight, size_t srcStride, const _SrcType *srcData,
    size_t dstWidth, size_t dstHeight, size_t dstStride, _DstType *dstData,
    const float *scale)
{
    const _SrcType *pixel;
    _DstType *dstPos;
    int dy;
    int dx;

    for(size_t y=0; y<dstHeight; ++y)
    {
        dstPos=dstData+(y*dstStride);
        dy=(int)((float)y*scale[1])*srcStride;

        for(size_t x=0; x<dstWidth; ++x)
        {
            dx=(int)((float)x*scale[0])*channels;
            pixel=&srcData[dy+dx];
            
            for(size_t i=0; i<channels; ++i)
                *(dstPos+i)=(_DstType)pixel[i];
            dstPos+=channels;
        }
    }

    return true;
}

template<size_t channels>
bool scale_NearestNeighbor_Channels(Depth srcDepth, size_t srcWidth, size_t srcHeight, size_t srcStride, const uint8_t *srcData,
    Depth dstDepth, size_t dstWidth, size_t dstHeight, size_t dstStride, uint8_t *dstData, const float *scale)
{
    if(srcDepth == Depth::Bit8)
    {
        if(dstDepth == Depth::Bit8)
        {
            return scale_NearestNeighbor_Packed<uint8_t, uint8_t, channels>(srcWidth, srcHeight, srcStride, srcData,
                dstWidth, dstHeight, dstStride, dstData, scale);
        }
        else if(dstDepth == Depth::Float)
        {
            return scale_NearestNeighbor_Packed<uint8_t, float, channels>(srcWidth, srcHeight, srcStride, srcData,
                dstWidth, dstHeight, dstStride, (float *)dstData, scale);
        }
    }
    else if(srcDepth == Depth::Float)
    {
        if(dstDepth == Depth::Bit8)
        {
            return scale_NearestNeighbor_Packed<float, uint8_t, channels>(srcWidth, srcHeight, srcStride, (float *)srcData,
                dstWidth, dstHeight, dstStride, dstData, scale);
        }
        else if(dstDepth == Depth::Float)
        {
            return scale_NearestNeighbor_Packed<float, float, channels>(srcWidth, srcHeight, srcStride, (float *)srcData,
                dstWidth, dstHeight, dstStride, (float *)dstData, scale);
        }
    }

    assert(false);//not implemented
    return false;
}

bool scale_NearestNeighbor(Format srcFormat, Depth srcDepth, size_t srcWidth, size_t srcHeight, size_t srcStride, const uint8_t *srcData,
    Format dstFormat, Depth dstDepth, size_t dstWidth, size_t dstHeight, size_t dstStride, uint8_t *dstData)
{
    const float scale[2] = {float(srcWidth)/float(dstWidth),
                                float(srcHeight)/float(dstHeight)};

    if(srcFormat == Format::RGB)
    {
        if(dstFormat == Format::RGB)
        {
            return scale_NearestNeighbor_Channels<3>(srcDepth, srcWidth, srcHeight, srcStride, srcData,
                dstDepth, dstWidth, dstHeight, dstStride, dstData, scale);
        }
    }
    else if(srcFormat == Format::RGBA)
    {
        if(dstFormat == Format::RGBA)
        {
            return scale_NearestNeighbor_Channels<4>(srcDepth, srcWidth, srcHeight, srcStride, srcData,
                dstDepth, dstWidth, dstHeight, dstStride, dstData, scale);
        }
    }

    assert(false);//not implemented
    return false;
}

namespace details
{

bool scale(Format srcFormat, Depth srcDepth, size_t srcWidth, size_t srcHeight, size_t srcStride, const uint8_t *srcData, size_t srcDataSize,
    Format dstFormat, Depth dstDepth, size_t dstWidth, size_t dstHeight, size_t dstStride, uint8_t *dstData, size_t dstDataSize,
    Interpolation interpolation)
{
    if(!srcData || !dstData)
        return false;

    if( srcWidth == 0 || dstWidth == 0 || srcHeight == 0 || dstHeight == 0 )
        return false;

//#ifdef IMGLIB_USE_FASTCV
//    if(fastcv::scale(srcFormat, srcWidth, srcHeight, srcStride, srcData, srcDataSize,
//        dstFormat, dstWidth, dstHeight, dstStride, dstData, dstDataSize,
//        interpolation))
//        return true;
//#endif//IMGLIB_USE_FASTCV

    if(interpolation == Interpolation::NearestNeighbor)
        return scale_NearestNeighbor(srcFormat, srcDepth, srcWidth, srcHeight, srcStride, srcData,
                dstFormat, dstDepth, dstWidth, dstHeight, dstStride, dstData);

    assert(false);//not implemented
    return false;
}

}//namespace details

}//namespace imglib