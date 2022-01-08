#ifndef _imglib_draw_h_
#define _imglib_draw_h_

#include <cassert>
#include <utility>

#define _USE_MATH_DEFINES
#include <math.h>

#include "imglib_export.h"
#include "imglib/image.h"

namespace imglib
{

template<typename _Type>
void clamp(_Type &value, _Type min, _Type max)
{
    if(value<min)
        value=min;
    if(value>max)
        value=max;
}

template<typename _Image, typename _Point>
bool valid(_Image &image, _Point &point)
{
    return ((vec::x(point)>=0.0f)&&(vec::x(point)<width(image))&&
        (vec::y(point)>=0.0f)&&(vec::y(point)<height(image)));
}

template<typename _Image, typename _Point, typename _Color>
void drawPoint(const _Point &point, const _Color &color, _Image &image)
{
    size_t imgStride=stride(image);
    uint8_t *imgData=data(image);
    Format imgFormat=format(image);

    size_t imgPixelSize=sizeOfPixel(imgFormat, depth(image));

    int ptX=vec::x(point);
    int ptY=vec::y(point);

    size_t imgWidth=width(image);
    size_t imgHeight=height(image);

    clamp(ptX, 0, (int)imgWidth-1);
    clamp(ptY, 0, (int)imgHeight-1);

    uint8_t *ptr=imgData+(imgStride*ptY)+(imgPixelSize*ptX);

    if(sizeof(color)==imgPixelSize)
    {
        ptr[0]=vec::red(color);
        ptr[1]=vec::green(color);
        ptr[2]=vec::blue(color);
        ptr[3]=vec::alpha(color);
    }
    else
        assert(false);
}


template<typename _Image, typename _Point, typename _Color>
void drawLine(const _Point &pt0, const _Point &pt1, const _Color &color, _Image &image)
{
    size_t imgStride=stride(image);
    uint8_t *imgData=data(image);
    Format imgFormat=format(image);
    size_t imgPixelSize=sizeOfPixel(imgFormat, depth(image));
    size_t imgWidth=width(image);
    size_t imgHeight=height(image);

    auto ipart=[](float x) -> int { return int(std::floor(x)); };
    auto round=[](float x) -> float { return std::round(x); };
    auto fpart=[](float x) -> float { return x-std::floor(x); };
    auto rfpart=[=](float x) -> float { return 1-fpart(x); };
    auto plot=[&](int x, int y, float brightness) -> void
    {
        if((x<0) || (x>imgWidth-1))
            return;
        if((y<0) || (y>imgHeight-1))
            return;

        uint8_t *ptr=imgData+(imgStride*y)+(imgPixelSize*x);

        ptr[0]=(ptr[0]*(1.0f-brightness))+vec::red(color)*brightness;
        ptr[1]=(ptr[1]*(1.0f-brightness))+vec::green(color)*brightness;
        ptr[2]=(ptr[2]*(1.0f-brightness))+vec::blue(color)*brightness;
        ptr[3]=(ptr[3]*(1.0f-brightness))+vec::alpha(color)*brightness;
//        *((_Color *)ptr)=color*brightness;
    };

    float x0=(float)vec::x(pt0);
    float y0=(float)vec::y(pt0);
    float x1=(float)vec::x(pt1);
    float y1=(float)vec::y(pt1);

//    clamp(x0, 0.0f, (float)(imgWidth-1));
//    clamp(y0, 0.0f, (float)(imgHeight-1));
//    clamp(x1, 0.0f, (float)(imgWidth-1));
//    clamp(y1, 0.0f, (float)(imgHeight-1));

    const bool steep=abs(y1-y0)>abs(x1-x0);
    if(steep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if(x0>x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    const float dx=x1-x0;
    const float dy=y1-y0;
    const float gradient=(dx==0.0f)?1:dy/dx;

    int xpx11;
    float intery;
    int xpx12;

    {
        const float xend=round(x0);
        const float yend=y0+gradient*(xend-x0);
        const float xgap=rfpart(x0+0.5);
        xpx11=int(xend);
        int ypx11=ipart(yend);

        if(steep)
        {
            plot(ypx11, xpx11, rfpart(yend) * xgap);
            plot(ypx11+1, xpx11, fpart(yend) * xgap);
        }
        else
        {
            plot(xpx11, ypx11, rfpart(yend) * xgap);
            plot(xpx11, ypx11+1, fpart(yend) * xgap);
        }
        intery=yend+gradient;
    }
    {
        const float xend=round(x1);
        const float yend=y1+gradient*(xend-x1);
        const float xgap=rfpart(x1+0.5);
        xpx12=int(xend);
        int ypx12=ipart(yend);

        if(steep)
        {
            plot(ypx12, xpx12, rfpart(yend) * xgap);
            plot(ypx12+1, xpx12, fpart(yend) * xgap);
        }
        else
        {
            plot(xpx12, ypx12, rfpart(yend) * xgap);
            plot(xpx12, ypx12+1, fpart(yend) * xgap);
        }
    }

    if(steep)
    {
        for(int x=xpx11+1; x<xpx12; x++)
        {
            plot(ipart(intery), x, rfpart(intery));
            plot(ipart(intery)+1, x, fpart(intery));
            intery+=gradient;
        }
    }
    else
    {
        for(int x=xpx11+1; x<xpx12; x++)
        {
            plot(x, ipart(intery), rfpart(intery));
            plot(x, ipart(intery)+1, fpart(intery));
            intery+=gradient;
        }
    }
}

template<typename _Image, typename _Point, typename _Color>
void drawArrow(_Image &image, const _Point &pt0, const _Point &pt1, const _Color &color, float arrowLength=25.0f)
{
    const float arrowSize=length(pt0-pt1)*arrowLength;

    if(valid(image, pt0) && valid(image, pt1))
        drawLine(pt0, pt1, color, image);

    float angle=atan2(y(pt0)-y(pt1), x(pt0)-x(pt1));
    _Point p(x(pt1)+arrowSize*cos(angle+M_PI/4.0f),
        y(pt1)+arrowSize*sin(angle+M_PI/4.0f));
    
    if(valid(image, p)&&valid(image, pt1))
        drawLine(p, pt1, color, image);

    x(p)=x(pt1)+arrowSize*cos(angle-M_PI/4.0f);
    y(p)=y(pt1)+arrowSize*sin(angle-M_PI/4.0f);
    
    if(valid(image, p)&&valid(image, pt1))
        drawLine(p, pt1, color, image);
}

template<typename _Image, typename _Point, typename _Color>
void drawAxisAlignedBox(_Image &image, const _Point &pt0, const _Point &pt1, const _Color &color)
{
    typename vec::traits::Value<_Point>::Type minX=std::min(vec::x(pt0), vec::x(pt1));
    typename vec::traits::Value<_Point>::Type maxX=std::max(vec::x(pt0), vec::x(pt1));
    typename vec::traits::Value<_Point>::Type minY=std::min(vec::y(pt0), vec::y(pt1));
    typename vec::traits::Value<_Point>::Type maxY=std::max(vec::y(pt0), vec::y(pt1));

    _Point topLeft(minX, minY);
    _Point topRight(maxX, minY);
    _Point bottomLeft(minX, maxY);
    _Point bottomRight(maxX, maxY);
    
    drawLine(topLeft, topRight, color, image);
    drawLine(topRight, bottomRight, color, image);
    drawLine(bottomLeft, bottomRight, color, image);
    drawLine(topLeft, bottomLeft, color, image);
}

}
#endif //_imglib_draw_h_