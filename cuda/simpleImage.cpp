#include "imglib/cuda/simpleImage.h"

#include <cuda_runtime.h>

#include <cassert>

namespace imglib
{
namespace cuda
{

SimpleImage::SimpleImage(Format format, Depth depth, size_t width, size_t height)
{
    allocData(format, depth, width, height);
}

SimpleImage::SimpleImage(Format format, Depth depth, size_t width, size_t height, uint8_t *data, size_t dataSize)
{
    allocData(format, depth, width, height);
    write(data, format, depth, width, height);
}

SimpleImage::~SimpleImage()
{
    freeData();
}

void SimpleImage::allocData(Format format, Depth depth, size_t width, size_t height, bool optimizeStride)
{
    if((m_format == format) && (m_depth == depth) && (m_width == width) && (m_height == height)
        && (m_dataSize > 0))
    {
        //image already suuports size
        return;
    }

    freeData();

    m_format=format;
    m_depth=depth;
    m_width=width;
    m_height=height;

    size_t widthInBytes=width*sizeOfPixel(m_format, m_depth);

    m_dataSize=widthInBytes*m_height;

    cudaError_t error;

    if(optimizeStride)
        error=cudaMallocPitch(&m_data, &m_stride, widthInBytes, height);
    else
    {
        error=cudaMalloc(&m_data, m_dataSize);
        m_stride=widthInBytes;
    }

    if(error != cudaSuccess)
    {
        m_data=nullptr;
        m_format=Format::Unknown;
        m_depth=Depth::Unknown;
        m_dataSize=0;
        m_width=0;
        m_height=0;
    }
}

void SimpleImage::freeData()
{
    if(m_data)
    {
        cudaFree(m_data);
        m_data=nullptr;
    }

    m_format=Format::Unknown;
    m_depth=Depth::Unknown;
    m_dataSize=0;
    m_width=0;
    m_height=0;
}

bool SimpleImage::write(unsigned char *buffer, Format format, Depth depth, unsigned int width, unsigned int height)
{
    if(width <= 0)
        return false;
    if(height <= 0)
        return false;

    if((m_format != format) || (m_depth != depth) || (m_width != width) || (m_height != height))
    {
        allocData(format, depth, width, height);
    }

    size_t widthInBytes=width*sizeOfPixel(format, depth);

    auto error=cudaMemcpy2D(m_data, m_stride, buffer, widthInBytes, widthInBytes, height, cudaMemcpyHostToDevice);

    if(error != cudaSuccess)
        return false;

    return true;
}

bool SimpleImage::read(unsigned char *buffer, Format format, Depth depth, unsigned int width, unsigned int height)
{
    if(width <= 0)
        return false;
    if(height <= 0)
        return false;

    if(m_format != format)
        return false;
    if(m_depth != depth)
        return false;
    if((m_width != width) || (m_height != height))
        return false;

    size_t widthInBytes=width*sizeOfPixel(format, depth);
    
    auto error=cudaMemcpy2D(buffer, width, m_data, m_stride, widthInBytes, height, cudaMemcpyDeviceToHost);
    
    if(error != cudaSuccess)
        return false;

    return true;
}

//bool SimpleImage::copy(SimpleImage &srcImage)
//{
//    if(width <= 0)
//        return false;
//    if(height <= 0)
//        return false;
//
//    if(m_format != srcImage.m_format)
//        return false;
//    if(m_depth != srcImage.m_depth)
//        return false;
//    if((m_width != srcImage.m_width) || (m_height != srcImage.m_height))
//        return false;
//
//    cudaMemcpy2D(m_data, m_stride, srcImage.data, srcImage.m_stride, m_width, m_height, cudaMemcpyDeviceToDevice);
//
//    if(error != cudaSuccess)
//        return false;
//
//    return true;
//}

}//namespace cuda
}//namespace imglib
