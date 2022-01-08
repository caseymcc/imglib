#ifndef _imglib_pixelOperations_
#define _imglib_pixelOperations_

#include "imglib/format_type.h"

namespace imglib
{

template<typename _SrcType, typename _SrcFormat, typename _DstType, typename _DstFormat>
void copyPixel(const _SrcType *src, _DstType *dst)
{
}

template<>
void copyPixel<uint8_t, Format_GreyScale, uint8_t, Format_GreyScale>(const uint8_t *src, uint8_t *dst)
{
    dst[0]=src[0];
}

template<>
void copyPixel<uint8_t, Format_RGB, uint8_t, Format_RGB>(const uint8_t *src, uint8_t *dst)
{
    dst[0]=src[0];
    dst[1]=src[1];
    dst[2]=src[2];
}

template<>
void copyPixel<float, Format_GreyScale, float, Format_GreyScale>(const float *src, float *dst)
{
    dst[0]=src[0];
}

template<>
void copyPixel<float, Format_RGB, float, Format_RGB>(const float *src, float *dst)
{
    dst[0]=src[0];
    dst[1]=src[1];
    dst[2]=src[2];
}

}


#endif//_imglib_pixelOperations_