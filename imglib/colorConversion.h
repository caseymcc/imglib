#ifndef _imglib_colorConversion_h_
#define _imglib_colorConversion_h_

#include "imglib_export.h"
#include "imglib/image.h"

#include <cstdint>

namespace imglib
{

template<typename _Image>
bool convert(_Image &srcImage, _Image &dstImage);

template<typename _Image>
bool rgb_to_rgba(_Image &srcImage, _Image &dstImage);

IMGLIB_EXPORT bool rgb_to_rgba(uint8_t *srcBuffer, size_t srcSize, Depth srcDepth, size_t srcWidth, size_t srcHeight, 
    uint8_t *dstBuffer, size_t dstSize, Depth dstDepth, size_t dstWidth, size_t dstHeight);

template<typename _Image>
bool rgb_to_greyscale(_Image &srcImage, _Image &dstImage);

IMGLIB_EXPORT bool rgb_to_greyscale(uint8_t *srcBuffer, size_t srcSize, Depth srcDepth, size_t srcWidth, size_t srcHeight, 
    uint8_t *dstBuffer, size_t dstSize, Depth dstDepth, size_t dstWidth, size_t dstHeight);

inline uint8_t clamp_uint8(int32_t value)
{
    if(value > 255)
        return 255;
    if(value < 0)
        return 0;
    return value;
}

template<typename _SrcImage, typename _DstImage>
bool yuv420sp_to_rgb(_SrcImage &srcImage, _DstImage &dstImage);

IMGLIB_EXPORT bool yuv420sp_to_rgb(uint8_t *srcBuffer, size_t srcSize, Depth srcDepth, size_t srcWidth, size_t srcHeight, 
    uint8_t *dstBuffer, size_t dstSize, Depth dstDepth, size_t dstWidth, size_t dstHeight);

template<typename _SrcImage, typename _DstImage>
bool rgb_to_yuv420sp(_SrcImage &srcImage, _DstImage &dstImage);

IMGLIB_EXPORT bool rgb_to_yuv420sp(uint8_t *srcBuffer, size_t srcSize, Depth srcDepth, size_t srcWidth, size_t srcHeight, 
    uint8_t *dstBuffer, size_t dstSize, Depth dstDepth, size_t dstWidth, size_t dstHeight);

}//namespace imglib

//source
#include "colorConversion.inl"

#endif //_imglib_colorConversion_h_