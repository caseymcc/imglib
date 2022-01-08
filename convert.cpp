#include "imglib/convert.h"

namespace imglib
{

bool convertNormMean_NearestNeighbor_Plane(const uint8_t *input, size_t inputWidth, size_t inputStride, 
    float *output, size_t outputWidth, const size_t outputHeight, size_t outputStride,
    const float *scale, float multiplier, float minValue, const float *mean, const float *stdDev)
{
    const int outputPlaneStride=outputWidth*outputHeight;
    float *channel0=output;
    float *channel1=channel0+outputPlaneStride;
    float *channel2=channel1+outputPlaneStride;

    for(size_t y=0; y<outputHeight; ++y)
    {
        const int dy=(int)((float)y*scale[1])*inputWidth;

        for(size_t x=0; x<outputWidth; ++x)
        {
            const int dx=((float)x*scale[0]);
            const uint8_t *pixel=&input[dy+dx];

            (*channel0)=(((float)pixel[0]*multiplier+minValue)-mean[0])/stdDev[0];
            (*channel1)=(((float)pixel[1]*multiplier+minValue)-mean[1])/stdDev[1];
            (*channel2)=(((float)pixel[2]*multiplier+minValue)-mean[2])/stdDev[2];

            channel0++;
            channel1++;
            channel2++;
        }
    }
    return true;
}

bool convertNormMean_NearestNeighbor_Packed(const uint8_t *input, size_t inputWidth, size_t inputStride, 
    float *output, size_t outputWidth, size_t outputHeight, size_t outputStride,
    const float *scale, float multiplier, float minValue, const float *mean, const float *stdDev)
{
    float *outputPos=output;

    for(size_t y=0; y<outputHeight; ++y)
    {
        const int dy=(int)((float)y*scale[1])*inputStride;

        for(size_t x=0; x<outputWidth; ++x)
        {
            const int dx=((float)x*scale[0])*3;//channels
            const uint8_t *pixel=&input[dy+dx];
            
            *(outputPos)  =(((float)pixel[0]*multiplier+minValue)-mean[0])/stdDev[0];
            *(outputPos+1)=(((float)pixel[1]*multiplier+minValue)-mean[1])/stdDev[1];
            *(outputPos+2)=(((float)pixel[2]*multiplier+minValue)-mean[2])/stdDev[2];

            outputPos+=3;
        }
    }

    return true;
}

bool convertNormMean_NearestNeighbor_Input(const uint8_t *input, size_t inputWidth, size_t inputStride, 
    float *output, Format outputFormat, size_t outputWidth, size_t outputHeight, size_t outputStride,
    const float *scale, float multiplier, float minValue, const float *mean, const float *stdDev)
{
    if(outputFormat == Format::RGB)
        return convertNormMean_NearestNeighbor_Packed(input, inputWidth, inputStride, output, outputWidth, outputHeight, outputStride, scale, multiplier, minValue, mean, stdDev);
    else if(outputFormat == Format::RGBP)
        return convertNormMean_NearestNeighbor_Plane(input, inputWidth, inputStride, output, outputWidth, outputHeight, outputStride, scale, multiplier, minValue, mean, stdDev);

    assert(false);//not implemented
    return false;
}

bool convertNormMean_NearestNeighbor(const void *input, Format inputFormat, size_t inputWidth, size_t inputHeight, size_t inputStride,
    float *output, Format outputFormat, size_t outputWidth, size_t outputHeight, size_t outputStride,
    const float *range, const float *mean, const float *stdDev)
{
    if(!input || !output)
        return false;

    if( inputWidth == 0 || outputWidth == 0 || inputHeight == 0 || outputHeight == 0 )
        return false;

    const float scale[2] = {float(inputWidth) / float(outputWidth),
                                float(inputHeight) / float(outputHeight)};

    const float multiplier = (range[1] - range[0]) / 255.0f;
    
    if(inputFormat == Format::RGB)
        return convertNormMean_NearestNeighbor_Input((uint8_t *)input, inputWidth, inputStride, output, outputFormat, outputWidth, outputHeight, outputStride, scale, multiplier, range[0], mean, stdDev);

    assert(false);//not implemented
    return false;
}

}//namespace imglib