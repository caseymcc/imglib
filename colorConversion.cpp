#include "imglib/colorConversion.h"

#include <cassert>
#include <algorithm>

namespace imglib
{

bool rgb_to_rgba(uint8_t *srcBuffer, size_t srcSize, Depth srcDepth, size_t srcWidth, size_t srcHeight,
    uint8_t *dstBuffer, size_t dstSize, Depth dstDepth, size_t dstWidth, size_t dstHeight)
{
    if(srcDepth != dstDepth)
        return false;

    size_t count=srcWidth*srcHeight;

    if(count > dstWidth*dstHeight)
        return false;

    size_t srcIndex=0;
    size_t dstIndex=0;

    for(size_t i=0; i<count; ++i)
    {
        dstBuffer[dstIndex]=srcBuffer[srcIndex];
        dstBuffer[dstIndex++]=srcBuffer[srcIndex++];
        dstBuffer[dstIndex++]=srcBuffer[srcIndex++];
        dstBuffer[dstIndex++]=255;
   }
    
    return true;
}

bool rgb_to_greyscale(uint8_t *srcBuffer, size_t srcSize, Depth srcDepth, size_t srcWidth, size_t srcHeight, 
    uint8_t *dstBuffer, size_t dstSize, Depth dstDepth, size_t dstWidth, size_t dstHeight)
{
    if(srcDepth != dstDepth)
        return false;

    size_t count=srcWidth*srcHeight;

    if(count > dstWidth*dstHeight)
        return false;

    float r, g, b;
    uint8_t *src=srcBuffer;
    uint8_t *dst=dstBuffer;

    for(size_t i=0; i<count; ++i)
    {
        r=(float)src[0];
        g=(float)src[1];
        b=(float)src[2];

        (*dst)=(uint8_t)((r*0.2126f)+(g*0.7152f)+(b*0.0722f));
        src+=3;
        dst++;
    }
    
    return true;
}

bool yuv420sp_to_rgb(uint8_t *srcBuffer, size_t srcSize, Depth srcDepth, size_t srcWidth, size_t srcHeight, 
    uint8_t *dstBuffer, size_t dstSize, Depth dstDepth, size_t dstWidth, size_t dstHeight)
{
    size_t halfWidth=srcWidth/2;
    size_t halfHeight=srcHeight/2;

    size_t srcIndex=0;
    size_t srcIndex2=srcWidth;
    size_t chromaIndex=srcWidth*srcHeight;
    size_t dstStride=dstWidth*3;
    size_t dstIndex=0;
    size_t dstIndex2=dstStride;
    
    for(size_t y=0; y<halfHeight; ++y)
    {
        for(size_t x=0; x<halfWidth; ++x)
        {
            int y11=(149*std::max(16, (int)srcBuffer[srcIndex]))>>1;
            int y12=(149*std::max(16, (int)srcBuffer[srcIndex+1]))>>1;
            int y21=(149*std::max(16, (int)srcBuffer[srcIndex2]))>>1;
            int y22=(149*std::max(16, (int)srcBuffer[srcIndex2+1]))>>1;

            int u=srcBuffer[chromaIndex];
            int v=srcBuffer[chromaIndex+1];

            int uvR=-14248+(102*v);
            int uvG=8663-(25*u)-(52*v);
            int uvB=-17705+(129*u);

            dstBuffer[dstIndex]=clamp_uint8((((y11+uvR)>>1)+16)>>5);
            dstBuffer[dstIndex+1]=clamp_uint8((((y11+uvG)>>1)+16)>>5);
            dstBuffer[dstIndex+2]=clamp_uint8((((y11+uvB)>>1)+16)>>5);
            
            dstBuffer[dstIndex+3]=clamp_uint8((((y12+uvR)>>1)+16)>>5);
            dstBuffer[dstIndex+4]=clamp_uint8((((y12+uvG)>>1)+16)>>5);
            dstBuffer[dstIndex+5]=clamp_uint8((((y12+uvB)>>1)+16)>>5);

            dstBuffer[dstIndex2]=clamp_uint8((((y21+uvR)>>1)+16)>>5);
            dstBuffer[dstIndex2+1]=clamp_uint8((((y21+uvG)>>1)+16)>>5);
            dstBuffer[dstIndex2+2]=clamp_uint8((((y21+uvB)>>1)+16)>>5);
            
            dstBuffer[dstIndex2+3]=clamp_uint8((((y22+uvR)>>1)+16)>>5);
            dstBuffer[dstIndex2+4]=clamp_uint8((((y22+uvG)>>1)+16)>>5);
            dstBuffer[dstIndex2+5]=clamp_uint8((((y22+uvB)>>1)+16)>>5);

            srcIndex+=2;
            srcIndex2+=2;
            chromaIndex+=2;
            dstIndex+=6;
            dstIndex2+=6;
        }
        srcIndex+=srcWidth;
        srcIndex2+=srcWidth;
        dstIndex+=dstStride;
        dstIndex2+=dstStride;
    }

    return true;
}

bool rgb_to_yuv420sp(uint8_t *srcBuffer, size_t srcSize, Depth srcDepth, size_t srcWidth, size_t srcHeight, 
    uint8_t *dstBuffer, size_t dstSize, Depth dstDepth, size_t dstWidth, size_t dstHeight)
{
    size_t halfWidth=srcWidth/2;
    size_t halfHeight=srcHeight/2;
    size_t srcStride=srcWidth*3;

    size_t srcIndex=0;
    size_t srcIndex2=srcStride;
    size_t dstIndex=0;
    size_t dstIndex2=dstWidth;
    size_t chromaIndex=dstWidth*dstHeight;
    
    for(size_t y=0; y<halfHeight; ++y)
    {
        for(size_t x=0; x<halfWidth; ++x)
        {
            int r=srcBuffer[srcIndex];
            int g=srcBuffer[srcIndex+1];
            int b=srcBuffer[srcIndex+2];
            int accR=r;
            int accG=g;
            int accB=b;

            dstBuffer[dstIndex]=clamp_uint8(((r*4899)+(g*9617)+(b*1868)+(1<<13))>>14);

            r=srcBuffer[srcIndex+3];
            g=srcBuffer[srcIndex+4];
            b=srcBuffer[srcIndex+5];
            accR+=r;
            accG+=g;
            accB+=b;

            dstBuffer[dstIndex+1]=clamp_uint8(((r*4899)+(g*9617)+(b*1868)+(1<<13))>>14);

            r=srcBuffer[srcIndex2];
            g=srcBuffer[srcIndex2+1];
            b=srcBuffer[srcIndex2+2];
            accR+=r;
            accG+=g;
            accB+=b;

            dstBuffer[dstIndex2]=clamp_uint8(((r*4899)+(g*9617)+(b*1868)+(1<<13))>>14);

            r=srcBuffer[srcIndex2+3];
            g=srcBuffer[srcIndex2+4];
            b=srcBuffer[srcIndex2+5];
            accR+=r;
            accG+=g;
            accB+=b;

            dstBuffer[dstIndex2+1]=clamp_uint8(((r*4899)+(g*9617)+(b*1868)+(1<<13))>>14);

            accR=accR>>2;
            accG=accG>>2;
            accB=accB>>2;

            dstBuffer[chromaIndex]=clamp_uint8(128+((-(accR*2765)-(accG*5427)+(accB*8192)+(1<<13))>>14));
            dstBuffer[chromaIndex+1]=clamp_uint8(128+(((accR*8192)-(accG*6860)-(accB*1332)+(1<<13))>>14));

            srcIndex+=6;
            srcIndex2+=6;
            dstIndex+=2;
            dstIndex2+=2;
            chromaIndex+=2;
        }
        srcIndex+=srcStride;
        srcIndex2+=srcStride;
        dstIndex+=dstWidth;
        dstIndex2+=dstWidth;
    }
    return true;
}

}//namespace imglib
