#ifndef _imglib_cuda_simpleImage_h_
#define _imglib_cuda_simpleImage_h_

#include "imglib/image.h"
#include "imglib_export.h"
#include "imglib/cuda/copy.h"

#include <vector>

namespace imglib
{
namespace cuda
{
/// A gpu image class provided by the library for quick use.
class IMGLIB_EXPORT SimpleImage
{
public:
    SimpleImage():m_width(0), m_stride(0), m_height(0){};
    SimpleImage(Format format, Depth depth, size_t width, size_t height);
    SimpleImage(Format format, Depth depth, size_t width, size_t height, uint8_t *data, size_t dataSize);
    template<typename _Image>
    SimpleImage(_Image &image, bool copyImage)
    {
        allocData(format(image), depth(image), width(image), height(image));

        if(copyImage)
            copy(image);
    }
    ~SimpleImage();

    Format format() const {return m_format;}
    Depth depth() const {return m_depth;}
    size_t width() const {return m_width;}
    size_t height() const {return m_height;}
    size_t stride() const {return m_stride;}
    uint8_t *data() const {return (uint8_t *)m_data;}
    size_t dataSize() const {return m_dataSize;}

    void allocData(Format format, Depth depth, size_t width, size_t height, bool optimizeStride=true);
    void freeData();

    bool write(unsigned char *buffer, Format format, Depth depth, unsigned int width, unsigned int height);
    bool read(unsigned char *buffer, Format format, Depth depth, unsigned int width, unsigned int height);
    
    template<typename _Image>
    bool copy(_Image &image)
    {
        if(location(image)==Location::System)
            return write(data(image), format(image), depth(image), width(image), height(image));
        else if(location(image)==Location::Cuda)
            return cuda::copy(*this, image);
        return false;
    }

private:
    Format m_format;
    Depth m_depth;

    size_t m_width;
    size_t m_height;
    size_t m_stride;

    void *m_data;
    size_t m_dataSize;
};

}//namespace cuda
}//namespace imglib

namespace imglib
{
template<>
inline Format format<cuda::SimpleImage>(const cuda::SimpleImage &image) { return image.format(); }
template<>
inline Depth depth<cuda::SimpleImage>(const cuda::SimpleImage &image) { return image.depth(); }
template<>
inline Location location<cuda::SimpleImage>(const cuda::SimpleImage &image) { return Location::Cuda; } //allways gpu for this guy

template<>
inline size_t width<cuda::SimpleImage>(const cuda::SimpleImage &image) { return image.width(); }
template<>
inline size_t height<cuda::SimpleImage>(const cuda::SimpleImage &image) { return image.height(); }
template<>
inline size_t stride<cuda::SimpleImage>(const cuda::SimpleImage &image) { return image.stride(); }

template<>
inline size_t nativeId<cuda::SimpleImage>(const cuda::SimpleImage &image) { return (size_t)image.data(); }

template<>
inline uint8_t *data<cuda::SimpleImage>(cuda::SimpleImage &image) { return (uint8_t *)image.data(); }
template<>
inline const uint8_t *data<cuda::SimpleImage>(const cuda::SimpleImage &image) { return (uint8_t *)image.data(); }
template<>
inline size_t dataSize<cuda::SimpleImage>(const cuda::SimpleImage &image) { return image.dataSize(); }

template<>
inline bool resize<cuda::SimpleImage>(cuda::SimpleImage &image, Format format, Depth depth, size_t width, size_t height)
{
    image.allocData(format, depth, width, height);
    return true;
}

}//namespace imglib

#endif //_imglib_cuda_simpleImage_h_