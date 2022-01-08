#include "imglib/bufferImage.h"

namespace imglib
{

BufferImage::BufferImage():
m_format(Format::Unknown),
m_depth(Depth::Unknown),
m_width(0),
m_height(0),
m_data(nullptr),
m_dataSize(0)
{}

BufferImage::BufferImage(Format format, Depth depth, size_t width, size_t height, uint8_t *data, size_t dataSize):
m_format(format),
m_depth(depth),
m_width(width),
m_height(height),
m_data(data),
m_dataSize(dataSize)
{
    m_stride=m_width*sizeOfPixel(format, depth);
}

BufferImage::BufferImage(Format format, Depth depth, size_t width, size_t stride, size_t height, uint8_t *data, size_t dataSize):
m_format(format),
m_depth(depth),
m_width(width),
m_stride(stride),
m_height(height),
m_data(data),
m_dataSize(dataSize)
{}

BufferImage::~BufferImage()
{}

void BufferImage::setBuffer(Format format, Depth depth, size_t width, size_t height, uint8_t *data, size_t dataSize)
{
    m_format=format;
    m_depth=depth;
    m_width=width;
    m_stride=m_width*sizeOfPixel(format, depth);
    m_height=height;
    m_data=data;
    m_dataSize=dataSize;
}

void BufferImage::setBuffer(Format format, Depth depth, size_t width, size_t stride, size_t height, uint8_t *data, size_t dataSize)
{
    m_format=format;
    m_depth=depth;
    m_width=width;
    m_stride=stride;
    m_height=height;
    m_data=data;
    m_dataSize=dataSize;
}

}//namespace imglib