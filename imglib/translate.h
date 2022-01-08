#ifndef _imglib_translate_h_
#define _imglib_translate_h_

#include "imglib/format_type.h"

namespace imglib
{

template<typename _SrcType, typename _SrcFormat, typename _DstType, typename _DstFormat>
void translatePixel(const _SrcType *src, _DstType *dst)
{}

template<>
void translatePixel<float, Format_RGB, uint8_t, Format_RGB>(const float *srcData, uint8_t *dstData)
{
    dstData[0]=srcData[0];
    dstData[1]=srcData[1];
    dstData[2]=srcData[2];
}

template<>
void translatePixel<uint8_t, Format_RGB, float, Format_RGB>(const uint8_t *srcData, float *dstData)
{
    dstData[0]=srcData[0];
    dstData[1]=srcData[1];
    dstData[2]=srcData[2];
}

}//namespace imglib

#endif//_imglib_translate_h_