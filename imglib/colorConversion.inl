#include <cassert>
#include <iostream>

#include "imglib/copy.h"

namespace imglib
{

template<typename _Image>
bool convert(_Image &srcImage, _Image &dstImage)
{
    if((location(srcImage) != Location::System) || (location(dstImage) != Location::System))
    {
        assert(false);
        return false;
    }

    switch(format(srcImage))
    {
    case Format::RGB:
    {
        switch(format(dstImage))
        {
        case Format::RGB:
            return copy(srcImage, dstImage);
        case Format::RGBA:
            return rgbToRGBA(srcImage, dstImage);
		case Format::GreyScale:
			return rgbToGreyScale(srcImage, dstImage);
        default:
            std::cout<<"Conversion from "<<formatToString(format(srcImage))<<" to "<<formatToString(format(dstImage))<<" not implemented";
            assert(false); //not implemented
            return false;
        }
    }
    break;
    default:
        assert(false); //not implemented
        return false;
    }

    return false;
}

template<typename _Image>
bool rgb_to_rgba(_Image &srcImage, _Image &dstImage)
{
    return rgb_to_rgba(data(srcImage), dataSize(srcImage), depth(srcImage), width(srcImage), height(srcImage),
        data(dstImage), dataSize(dstImage), depth(dstImage), width(dstImage), height(dstImage));
}

template<typename _Image>
bool rgb_to_greyscale(_Image &srcImage, _Image &dstImage)
{
    return rgb_to_greyscale(data(srcImage), dataSize(srcImage), depth(srcImage), width(srcImage), height(srcImage),
        data(dstImage), dataSize(dstImage), depth(dstImage), width(dstImage), height(dstImage));
}

template<typename _SrcImage, typename _DstImage>
bool yuv420sp_to_rgb(_SrcImage &srcImage, _DstImage &dstImage)
{
    return yuv420sp_to_rgb(data(srcImage), dataSize(srcImage), depth(srcImage), width(srcImage), height(srcImage),
        data(dstImage), dataSize(dstImage), depth(dstImage), width(dstImage), height(dstImage));
}

template<typename _SrcImage, typename _DstImage>
bool rgb_to_yuv420sp(_SrcImage &srcImage, _DstImage &dstImage)
{
    return rgb_to_yuv420sp(data(srcImage), dataSize(srcImage), depth(srcImage), width(srcImage), height(srcImage),
        data(dstImage), dataSize(dstImage), depth(dstImage), width(dstImage), height(dstImage));
}

}//namespace imglib
