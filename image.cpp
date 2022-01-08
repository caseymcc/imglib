#include "imglib/image.h"

#include <unordered_map>
#include <cstring>

namespace imglib
{

struct FormatDetails
{
    const char *name;
    size_t channels;
};

typedef std::unordered_map<Format, FormatDetails> FormatDetailsMap;
const FormatDetailsMap &getFormatDetails()
{
    static FormatDetailsMap formatDetails=
    {
        {Format::Unknown,{"Unknown", 0}},
        {Format::Binary,{"Binary", traits::channels<Format::Binary>()}},
        {Format::GreyScale,{"GreyScale", traits::channels<Format::GreyScale>()}},
        {Format::RA,{"RA", traits::channels<Format::RA>()}},
        {Format::RGB,{"RGB", traits::channels<Format::RGB>()}},
        {Format::RGBA,{"RGBA", traits::channels<Format::RGBA>()}},
        {Format::BGRA,{"BGRA", traits::channels<Format::BGRA>()}},
        {Format::YUV420,{"YUV420", traits::channels<Format::YUV420>()}},
        {Format::YUV420P,{"YUV420P", traits::channels<Format::YUV420P>()}},
        {Format::YUV420SP,{"YUV420SP", traits::channels<Format::YUV420SP>()}}
    };

    return formatDetails;
}

struct DepthDetails
{
    const char *name;
    size_t bits;
};

typedef std::unordered_map<Depth, DepthDetails> DepthDetailsMap;
const DepthDetailsMap &getDepthDetails()
{
    static DepthDetailsMap depthDetails=
    {
        {Depth::Unknown,{"Unknown", 0}},
        {Depth::Bit1,{"1 bit", 1}},
        {Depth::Bit8,{"8 bit", 8}},
        {Depth::Bit10,{"10 bit", 10}},
        {Depth::Bit12,{"12 bit", 12}},
        {Depth::Bit14,{"14 bit", 14}},
        {Depth::Bit16,{"16 bit", 16}},
        {Depth::Float,{"Float", 32}},
        {Depth::Double,{"Double", 64}}//,
    //    {Depth::Packed,{"Packed", traits::sizeOfDepth<Depth::Packed>()}}
    };

    return depthDetails;
}


const char *formatToString(Format format)
{
    const auto iter=getFormatDetails().find(format);

    if(iter==getFormatDetails().end())
        return "Unknown";
    return iter->second.name;
}

Format formatFromString(const char *name)
{
    for(const auto &iter:getFormatDetails())
    {
        if(strcmp(iter.second.name, name) == 0)
            return iter.first;
    }
    return Format::Binary;
}

size_t formatToChannels(Format format)
{
    const auto iter=getFormatDetails().find(format);

    if(iter==getFormatDetails().end())
        return 0;
    return iter->second.channels;
}

Format channelsToFormat(size_t channels)
{
    switch(channels)
    {
    case 1:
        return Format::GreyScale;
        break;
    case 2:
        return Format::RA;
        break;
    case 3:
        return Format::RGB;
        break;
    case 4:
        return Format::RGBA;
        break;
    }
    return Format::Unknown;
}

const char *depthToString(Depth depth)
{
    const auto iter=getDepthDetails().find(depth);

    if(iter==getDepthDetails().end())
        return "Unknown";
    return iter->second.name;
}

Depth depthFromString(const char *name)
{
    for(const auto &iter:getDepthDetails())
    {
        if(strcmp(iter.second.name, name)==0)
            return iter.first;
    }
    return Depth::Bit1;
}

size_t depthToBits(Depth depth)
{
    const auto iter=getDepthDetails().find(depth);

    if(iter==getDepthDetails().end())
        return 0;
    return iter->second.bits;
}

Depth bitsToDepth(size_t bits)
{
    for(const auto &iter:getDepthDetails())
    {
        if(iter.second.bits==bits)
            return iter.first;
    }
    return Depth::Unknown;
}

template<Format _Format>
size_t sizeOfImageDepth(Depth depth)
{
    switch(depth)
    {
    case Depth::Bit1:
        return traits::sizeOf<_Format, Depth::Bit1>();
    case Depth::Bit8:
        return traits::sizeOf<_Format, Depth::Bit8>();
    case Depth::Bit10:
        return traits::sizeOf<_Format, Depth::Bit10>();
    case Depth::Bit12:
        return traits::sizeOf<_Format, Depth::Bit12>();
    case Depth::Bit14:
        return traits::sizeOf<_Format, Depth::Bit14>();
    case Depth::Bit16:
        return traits::sizeOf<_Format, Depth::Bit16>();
    case Depth::Float16:
        return traits::sizeOf<_Format, Depth::Float16>();
    case Depth::Float:
        return traits::sizeOf<_Format, Depth::Float>();
    case Depth::Double:
        return traits::sizeOf<_Format, Depth::Double>();
    default:
        return 0;
    }
    return 0;
}

size_t sizeOfPixel(Format format, Depth depth)
{
    switch(format)
    {
    case Format::Binary:
        return sizeOfImageDepth<Format::Binary>(depth);
        break;
    case Format::GreyScale:
        return sizeOfImageDepth<Format::GreyScale>(depth);
        break;
    case Format::RA:
        return sizeOfImageDepth<Format::RA>(depth);
        break;
    case Format::RGB:
        return sizeOfImageDepth<Format::RGB>(depth);
        break;
    case Format::RGBA:
        return sizeOfImageDepth<Format::RGBA>(depth);
        break;
    case Format::YUV420:
        return sizeOfImageDepth<Format::YUV420>(depth);
        break;
    case Format::YUV420P:
        return sizeOfImageDepth<Format::YUV420P>(depth);
        break;
    case Format::YUV420SP:
        return sizeOfImageDepth<Format::YUV420SP>(depth);
        break;
    default:
        return 0;
    }
    return 0;
}


size_t imageStride(Format format, Depth depth, size_t width)
{
    switch(format)
    {
    case Format::YUV420:
    case Format::YUV420P:
    case Format::YUV420SP:
        return width;
        break;
    default:
        break;
    }
	return sizeOfPixel(format, depth)*width;
}

size_t sizeOfImage(Format format, Depth depth, size_t width, size_t height)
{
    switch(format)
    {
    case Format::YUV420:
    case Format::YUV420P:
    case Format::YUV420SP:
        return ((width*height)>>2)*6;
        break;
    default:
        break;
    }
	return sizeOfPixel(format, depth)*width*height;
}

}//namespace imglib
