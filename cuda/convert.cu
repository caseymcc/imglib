#include "imglib/cuda/convert.h"
#include "imglib/format_type.h"
#include "imglib/cuda/util.h"

#include "translate.cu"

namespace imglib
{
namespace cuda
{

//template<typename _Format>
//__device__ __host__ int formatChannels(){return 0;}
//
//template<>
//__device__ __host__ int formatChannels<Format_Binary>(){return 1;}
//template<>
//__device__ __host__ int formatChannels<Format_GreyScale>(){return 1;}
//template<>
//__device__ __host__ int formatChannels<Format_RA>(){return 2;}
//template<>
//__device__ __host__ int formatChannels<Format_RGB>(){return 3;}
//template<>
//__device__ __host__ int formatChannels<Format_RGBA>(){return 4;}
//template<>
//__device__ __host__ int formatChannels<Format_RGBP>(){return 3;}
//template<>
//__device__ __host__ int formatChannels<Format_RGBAP>(){return 4;}
//template<>
//__device__ __host__ int formatChannels<Format_BGR>(){return 3;}
//template<>
//__device__ __host__ int formatChannels<Format_BGRA>(){return 4;}
//template<>
//__device__ __host__ int formatChannels<Format_BGRP>(){return 3;}
//template<>
//__device__ __host__ int formatChannels<Format_BGRAP>(){return 4;}

//template<typename _OutputFormat, typename _InputType, typename _InputFormat>
//__global__ void convertNormMean_NearestNeighbor_Plane(_InputType *input, int inputWidth, float *output, int outputWidth, int outputHeight, float2 scale, float multiplier, float minValue, const float3 mean, const float3 stdDev)
__global__ void convertNormMean_NearestNeighbor_Plane(uchar3 *input, int inputWidth, float *output, int outputWidth, int outputHeight, float2 scale, float multiplier, float minValue, const float3 mean, const float3 stdDev)
{
    const int x=(blockIdx.x*blockDim.x)+threadIdx.x;
    const int y=(blockIdx.y*blockDim.y)+threadIdx.y;

    if((x >= outputWidth) || (y >= outputHeight))
        return;

//    const int outputChannels=_OutputFormat::channels;
    const int outputChannels=3;

    const int outputPlaneStride=outputWidth*outputHeight;
    const int outputPos=(y*outputWidth)+x;

    const int dx=((float)x*scale.x);
    const int dy=((float)y*scale.y);

    //const _InputType pixel=input[dy*inputWidth+dx];
    const uchar3 pixel=input[dy*inputWidth+dx];

//    const float3 outPixel=translatePixel<float3, _OutputFormat, _InputType, _InputFormat>(pixel);
    const float3 outPixel=make_float3(pixel.x, pixel.y, pixel.z);
    
    output[(outputPlaneStride*0)+outputPos]=((outPixel.x*multiplier+minValue)-mean.x)/stdDev.x;
    output[(outputPlaneStride*1)+outputPos]=((outPixel.y*multiplier+minValue)-mean.y)/stdDev.y;
    output[(outputPlaneStride*2)+outputPos]=((outPixel.z*multiplier+minValue)-mean.z)/stdDev.z;
}

//template<typename _OutputFormat, typename _InputType, typename _InputFormat>
//__global__ void convertNormMean_NearestNeighbor_Packed(_InputType *input, int inputWidth, float *output, int outputWidth, int outputHeight, float2 scale, float multiplier, float minValue, const float3 mean, const float3 stdDev)
__global__ void convertNormMean_NearestNeighbor_Packed(uchar3 *input, int inputWidth, float *output, int outputWidth, int outputHeight, float2 scale, float multiplier, float minValue, const float3 mean, const float3 stdDev)
{
    const int x=(blockIdx.x*blockDim.x)+threadIdx.x;
    const int y=(blockIdx.y*blockDim.y)+threadIdx.y;

    if((x >= outputWidth) || (y >= outputHeight))
        return;

//    const int outputChannels=_OutputFormat::channels;
    const int outputChannels=3;

    const int outputStride=outputWidth*outputChannels;
    const int outputPos=(y*outputStride)+(x*outputChannels);

    const int dx=((float)x*scale.x);
    const int dy=((float)y*scale.y);

//    const _InputType pixel=input[dy*inputWidth+dx];
    const uchar3 pixel=input[dy*inputWidth+dx];

//    const float3 outPixel=translatePixel<float3, _OutputFormat, _InputType, _InputFormat>(pixel);
    const float3 outPixel=make_float3(pixel.x, pixel.y, pixel.z);
    
    output[outputPos]=((outPixel.x*multiplier+minValue)-mean.x)/stdDev.x;
    output[outputPos+1]=((outPixel.y*multiplier+minValue)-mean.y)/stdDev.y;
    output[outputPos+2]=((outPixel.z*multiplier+minValue)-mean.z)/stdDev.z;
}

//template<typename _InputType, typename _InputFormat>
//cudaError_t convertNormMean_NearestNeighbor_Input(const dim3 &blockDim, const dim3 &gridDim, cudaStream_t stream,
//    _InputType *input, size_t inputWidth, 
//    float *output, Format outputFormat, size_t outputWidth, size_t outputHeight, const float2 &scale, float multiplier, float minValue, const float3 &mean, const float3 &stdDev)
cudaError_t convertNormMean_NearestNeighbor_Input(const dim3 &blockDim, const dim3 &gridDim, cudaStream_t stream,
    uchar3 *input, size_t inputWidth, 
    float *output, Format outputFormat, size_t outputWidth, size_t outputHeight, const float2 &scale, float multiplier, float minValue, const float3 &mean, const float3 &stdDev)
{
    if(outputFormat == Format::RGB)
//        convertNormMean_NearestNeighbor_Packed<Format_RGB, _InputType, _InputFormat><<<gridDim, blockDim, 0, stream>>>(input, (int)inputWidth, output, (int)outputWidth, (int)outputHeight, scale, multiplier, minValue, mean, stdDev);
//        convertNormMean_NearestNeighbor_Packed<<<gridDim, blockDim, 0, stream>>>(input, (int)inputWidth, output, (int)outputWidth, (int)outputHeight, scale, multiplier, minValue, mean, stdDev);
        convertNormMean_NearestNeighbor_Packed<<<gridDim, blockDim>>>(input, (int)inputWidth, output, (int)outputWidth, (int)outputHeight, scale, multiplier, minValue, mean, stdDev);
    else if(outputFormat == Format::RGBP)
//        convertNormMean_NearestNeighbor_Plane<Format_RGBP, _InputType, _InputFormat><<<gridDim, blockDim, 0, stream>>>(input, (int)inputWidth, output, (int)outputWidth, (int)outputHeight, scale, multiplier, minValue, mean, stdDev);
        convertNormMean_NearestNeighbor_Plane<<<gridDim, blockDim, 0, stream>>>(input, (int)inputWidth, output, (int)outputWidth, (int)outputHeight, scale, multiplier, minValue, mean, stdDev);
//    else if(outputFormat == Format::BGR)
//        convertNormMean_NearestNeighbor_Packed<Format_BGR, _InputType, _InputFormat><<<gridDim, blockDim, 0, stream>>>(input, (int)inputWidth, output, (int)outputWidth, (int)outputHeight, scale, multiplier, minValue, mean, stdDev);
//    else if(outputFormat == Format::BGRP)
//        convertNormMean_NearestNeighbor_Plane<Format_BGRP, _InputType, _InputFormat><<<gridDim, blockDim, 0, stream>>>(input, (int)inputWidth, output, (int)outputWidth, (int)outputHeight, scale, multiplier, minValue, mean, stdDev);
    else
    {   
        assert(false);//not implemented
        return cudaErrorInvalidValue;
    }

    return cudaGetLastError();
}

cudaError_t convertNormMean_NearestNeighbor(void *input, Format inputFormat, size_t inputWidth, size_t inputHeight,
    float *output, Format outputFormat, size_t outputWidth, size_t outputHeight, 
    const float2 &range, const float3& mean, const float3& stdDev, cudaStream_t stream)
{
    if(!input || !output)
        return cudaErrorInvalidDevicePointer;

    if( inputWidth == 0 || outputWidth == 0 || inputHeight == 0 || outputHeight == 0 )
        return cudaErrorInvalidValue;

    const float2 scale = make_float2( float(inputWidth) / float(outputWidth),
                                float(inputHeight) / float(outputHeight) );

    const float multiplier = (range.y - range.x) / 255.0f;
    
    // launch kernel
    const dim3 blockDim(8, 8);
    const dim3 gridDim(divCeil(outputWidth,blockDim.x), divCeil(outputHeight,blockDim.y));

    if(inputFormat == Format::RGB)
        return convertNormMean_NearestNeighbor_Input(blockDim, gridDim, stream, (uchar3 *)input, inputWidth, output, outputFormat, outputWidth, outputHeight, scale, multiplier, range.x, mean, stdDev);
//        return convertNormMean_NearestNeighbor_Input<uchar3, Format_RGB>(blockDim, gridDim, stream, (uchar3 *)input, inputWidth, output, outputFormat, outputWidth, outputHeight, scale, multiplier, range.x, mean, stdDev);
//    else if(inputFormat == Format::RGBA)
//        return convertNormMean_NearestNeighbor_Input<uchar4, Format_RGBA>(blockDim, gridDim, stream, (uchar4 *)input, inputWidth, output, outputFormat, outputWidth, outputHeight, scale, multiplier, range.x, mean, stdDev);
//    else if(inputFormat == Format::BGR)
//        return convertNormMean_NearestNeighbor_Input<uchar3, Format_BGR>(blockDim, gridDim, stream, (uchar3 *)input, inputWidth, output, outputFormat, outputWidth, outputHeight, scale, multiplier, range.x, mean, stdDev);
//    else if(inputFormat == Format::BGRA)
//        return convertNormMean_NearestNeighbor_Input<uchar4, Format_BGRA>(blockDim, gridDim, stream, (uchar4 *)input, inputWidth, output, outputFormat, outputWidth, outputHeight, scale, multiplier, range.x, mean, stdDev);

    assert(false);//not implemented
    return cudaErrorInvalidValue;
}

}//namespace cuda
}//namespace imglib