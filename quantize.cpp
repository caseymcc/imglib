#include "imglib/quantize.h"

#include <iostream>
#include <cmath>
#include <limits>

namespace imglib
{
namespace details
{

bool quantize(Format inputFormat, size_t inputWidth, size_t inputHeight, size_t inputStride, const float *input,
    Format outputFormat, Depth outputDepth, size_t outputWidth, size_t outputHeight, size_t outputStride, uint8_t *output)
{
    unsigned char stepEquivalentTo0;
    float quantizedStepSize;
    float trueMin = std::numeric_limits<float>::max();
    float trueMax = std::numeric_limits<float>::min();

    size_t elements=inputWidth*inputHeight*imglib::formatToChannels(inputFormat);

    for(size_t i=0; i<elements; ++i)
    {
        trueMin = std::min(trueMin, input[i]);
        trueMax = std::max(trueMax, input[i]);
    }

    size_t bitWidth=imglib::depthToBits(outputDepth);

    double encodingMin;
    double encodingMax;
    double stepCloseTo0;
    double trueBitWidthMax=std::pow(2, bitWidth)-1;

    if(trueMin > 0.0f)
    {
        stepCloseTo0=0.0;
        encodingMin=0.0;
        encodingMax=trueMax;
    }
    else if(trueMax < 0.0f)
    {
        stepCloseTo0=trueBitWidthMax;
        encodingMin=trueMin;
        encodingMax=0.0;
    }
    else
    {
        double trueStepSize=static_cast <double>(trueMax-trueMin)/trueBitWidthMax;

        stepCloseTo0=-trueMin / trueStepSize;

        if (stepCloseTo0 == round(stepCloseTo0))
        {
            // 0.0 is exactly representable
            encodingMin=trueMin;
            encodingMax=trueMax;
        }
        else
        {
            stepCloseTo0=round(stepCloseTo0);
            encodingMin=(0.0-stepCloseTo0)*trueStepSize;
            encodingMax=(trueBitWidthMax-stepCloseTo0)*trueStepSize;
        }
    }

    const double minEncodingRange=0.01;
    double encodingRange=encodingMax-encodingMin;

    quantizedStepSize=encodingRange/trueBitWidthMax;
    stepEquivalentTo0=static_cast<unsigned char>(round(stepCloseTo0));

    if(encodingRange < minEncodingRange)
    {
        std::cout<<"Expect the encoding range to be larger than " <<  minEncodingRange << "\n"
                << "Got: " << encodingRange << "\n";
        return false;
    }
    else
    {
        for(size_t i=0; i<elements; ++i)
        {
            int quantizedValue=round(trueBitWidthMax*(input[i]-encodingMin)/encodingRange);

            if(quantizedValue < 0)
                quantizedValue = 0;
            else if(quantizedValue > (int)trueBitWidthMax)
                quantizedValue = (int)trueBitWidthMax;

            if(bitWidth == 8)
            {
                output[i]=static_cast<uint8_t>(quantizedValue);
            }
            else if(bitWidth == 16)
            {
                uint16_t *temp=(uint16_t *)output;

                temp[i]=static_cast <uint16_t>(quantizedValue);
            }
        }
    }
    return true;
}

}//namespace details
}//namespace imglib
