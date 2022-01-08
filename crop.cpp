#include "imglib/crop.h"
#include "imglib/translate.h"
#include "imglib/math.h"

#include <algorithm>

namespace imglib
{

namespace details
{

bool crop(Format srcFormat, Depth srcDepth, size_t srcWidth, size_t srcHeight, size_t srcStride, const uint8_t *srcData, size_t srcDataSize,
    Format dstFormat, Depth dstDepth, size_t dstWidth, size_t dstHeight, size_t dstStride, uint8_t *dstData, size_t dstDataSize,
    int left, int right, int top, int bottom)
{
    int cropWidth=right-left+1;
    int cropHeight=bottom-top+1;

    if(dstWidth < cropWidth)
        return false;

    if(dstHeight < cropHeight)
        return false;

    if(srcFormat != dstFormat)
        return false;

    if(srcDepth != dstDepth)
        return false;

    left=clamp(left, 0, cropWidth-1);
    right=clamp(right, 0, cropWidth-1);
    top=clamp(top, 0, cropHeight-1);
    bottom=clamp(bottom, 0, cropHeight-1);

    size_t channels=formatToChannels(srcFormat);
    size_t srcPixelSize=sizeOfPixel(srcFormat, srcDepth);
    size_t srcIndex;
    size_t dstIndex;

    for(size_t y=top, dstY=0; y<=bottom; ++y, ++dstY)
    {
        srcIndex=(y*srcStride)+(left*srcPixelSize);
        dstIndex=dstY*dstStride;

        for(size_t x=left; x<=right; ++x)
        {
            for(size_t channel=0; channel<channels; ++channel)
                dstData[dstIndex+channel]=srcData[srcIndex+channel];

            srcIndex+=srcPixelSize;
            dstIndex+=srcPixelSize;
        }
    }

    return true;
}

}//namespace details
}//namespace imglib
