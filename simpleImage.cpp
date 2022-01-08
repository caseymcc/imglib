#include "imglib/simpleImage.h"

#include <algorithm>
#include <cstring>

namespace imglib
{

SimpleImage::SimpleImage(const SimpleImage &image)
{
    allocData(image.format(), image.depth(), image.width(), image.height());

    std::copy(image.m_data.begin(), image.m_data.end(), m_data.begin());
}

SimpleImage::SimpleImage(SimpleImage &&image) noexcept
{
    m_format=image.format();
    m_depth=image.depth();

    m_width=image.width();
    m_height=image.height();
    m_stride=image.stride();

    std::swap(m_data, image.m_data);
    
    image.freeData();
}

SimpleImage::SimpleImage(Format format, Depth depth, size_t width, size_t height)
{
    allocData(format, depth, width, height);
}

SimpleImage::SimpleImage(Format format, Depth depth, size_t width, size_t height, uint8_t *data, size_t dataSize)
{
    allocData(format, depth, width, height);

    if(m_stride != m_width*sizeOfPixel(format, depth))
        copy(format, depth, width, width*sizeOfPixel(format, depth), height, data, dataSize);
    else
        memcpy(m_data.data(), data, std::min(m_data.size(), dataSize));
}

SimpleImage::SimpleImage(Format format, Depth depth, size_t width, size_t stride, size_t height, uint8_t *data, size_t dataSize)
{
    copy(format, depth, width, stride, height, data, dataSize);
}

SimpleImage::~SimpleImage()
{
    freeData();
}

SimpleImage &SimpleImage::operator=(const SimpleImage &image)
{
    allocData(image.format(), image.depth(), image.width(), image.height());

    std::copy(image.m_data.begin(), image.m_data.end(), m_data.begin());
    return *this;
}

SimpleImage &SimpleImage::operator=(SimpleImage &&image) noexcept
{
    m_format=image.format();
    m_depth=image.depth();

    m_width=image.width();
    m_height=image.height();
    m_stride=image.stride();

    std::swap(m_data, image.m_data);

    image.freeData();
    return *this;
}

void SimpleImage::allocData(Format format, Depth depth, size_t width, size_t height)
{
    if((m_format == format) && (m_depth == depth) && (m_width == width) && (m_height == height)
        && (m_data.size() > 0))
    {
        //image already suports size
        return;
    }

    freeData();

    m_format=format;
    m_depth=depth;
    m_width=width;
    m_height=height;

    m_stride=imageStride(format, depth, width);
    m_data.resize(sizeOfImage(format, depth, width, height));
}

void SimpleImage::freeData()
{
    m_format=Format::Unknown;
    m_depth=Depth::Unknown;

    m_width=0;
    m_height=0;
    m_stride=0;

    m_data.clear();
}

void SimpleImage::copy(Format format, Depth depth, size_t width, size_t stride, size_t height, uint8_t *data, size_t dataSize)
{
    allocData(format, depth, width, height);

    uint8_t *localData=m_data.data();
    uint8_t *remoteData=data;
    uint8_t minStride=std::min(m_stride, stride);

    for(size_t y=0; y<height; ++y)
    {
        memcpy(localData, remoteData, minStride);
        localData+=m_stride;
        remoteData+=stride;
    }
}

}//namespace imglib
