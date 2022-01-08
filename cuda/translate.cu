#include "imglib/format_type.h"

namespace imglib
{

template<typename _OutPixelType, typename _OutFormat, typename _InPixelType, typename _InFormat>
__device__ _OutPixelType translatePixel(const _InPixelType &input)
{
    return _OutPixelType();
}

/////////////////////////////
//float3 output
/////////////////////////////
template<>
__device__ float3 translatePixel<float3, Format_RGB, uchar3, Format_RGB>(const uchar3 &pixel)
{
    return make_float3(pixel.x, pixel.y, pixel.z);
}

template<>
__device__ float3 translatePixel<float3, Format_RGB, uchar3, Format_BGR>(const uchar3 &pixel)
{
    return make_float3(pixel.z, pixel.y, pixel.x);
}

template<>
__device__ float3 translatePixel<float3, Format_RGB, uchar4, Format_RGBA>(const uchar4 &pixel)
{
    return make_float3(pixel.x, pixel.y, pixel.z);
}

template<>
__device__ float3 translatePixel<float3, Format_RGB, uchar4, Format_BGRA>(const uchar4 &pixel)
{
    return make_float3(pixel.z, pixel.y, pixel.x);
}

template<>
__device__ float3 translatePixel<float3, Format_BGR, uchar3, Format_RGB>(const uchar3 &pixel)
{
    return make_float3(pixel.z, pixel.y, pixel.x);
}

template<>
__device__ float3 translatePixel<float3, Format_BGR, uchar3, Format_BGR>(const uchar3 &pixel)
{
    return make_float3(pixel.x, pixel.y, pixel.z);
}

template<>
__device__ float3 translatePixel<float3, Format_BGR, uchar4, Format_RGB>(const uchar4 &pixel)
{
    return make_float3(pixel.z, pixel.y, pixel.x);
}

template<>
__device__ float3 translatePixel<float3, Format_BGR, uchar4, Format_BGRA>(const uchar4 &pixel)
{
    return make_float3(pixel.x, pixel.y, pixel.z);
}

/////////////////////////////
//float4 output
/////////////////////////////
template<>
__device__ float4 translatePixel<float4, Format_RGBA, uchar3, Format_RGB>(const uchar3 &pixel)
{
    return make_float4(pixel.x, pixel.y, pixel.z, 1.0f);
}

template<>
__device__ float4 translatePixel<float4, Format_RGBA, uchar3, Format_BGR>(const uchar3 &pixel)
{
    return make_float4(pixel.z, pixel.y, pixel.x, 1.0f);
}

template<>
__device__ float4 translatePixel<float4, Format_RGBA, uchar4, Format_RGBA>(const uchar4 &pixel)
{
    return make_float4(pixel.x, pixel.y, pixel.z, pixel.w);
}

template<>
__device__ float4 translatePixel<float4, Format_RGBA, uchar4, Format_BGRA>(const uchar4 &pixel)
{
    return make_float4(pixel.z, pixel.y, pixel.x, pixel.w);
}

template<>
__device__ float4 translatePixel<float4, Format_BGRA, uchar3, Format_RGB>(const uchar3 &pixel)
{
    return make_float4(pixel.z, pixel.y, pixel.x, 1.0f);
}

template<>
__device__ float4 translatePixel<float4, Format_BGRA, uchar3, Format_BGR>(const uchar3 &pixel)
{
    return make_float4(pixel.x, pixel.y, pixel.z, 1.0f);
}

template<>
__device__ float4 translatePixel<float4, Format_BGRA, uchar4, Format_RGB>(const uchar4 &pixel)
{
    return make_float4(pixel.z, pixel.y, pixel.x, pixel.w);
}

template<>
__device__ float4 translatePixel<float4, Format_BGRA, uchar4, Format_BGRA>(const uchar4 &pixel)
{
    return make_float4(pixel.x, pixel.y, pixel.z, pixel.w);
}

}//namespace imglib