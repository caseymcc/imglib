#ifndef _imglib_simpleImage_h_
#define _imglib_simpleImage_h_

#include "imglib/image.h"
#include "imglib_export.h"

#include <vector>

#ifdef IMGLIB_CUDA
#include "imglib/cuda/simpleImage.h"
#endif//IMGLIB_CUDA

namespace imglib
{

/// A simple image class provided by the library for quick use. The library can accept many different kinds of images
/// as long as certain functions are declared, see image.h
class IMGLIB_EXPORT SimpleImage
{
public:
    SimpleImage():m_width(0), m_stride(0), m_height(0){};
    SimpleImage(const SimpleImage &image);
    SimpleImage(SimpleImage &&image) noexcept;
    SimpleImage(Format format, Depth depth, size_t width, size_t height);
    SimpleImage(Format format, Depth depth, size_t width, size_t height, uint8_t *data, size_t dataSize);
    SimpleImage(Format format, Depth depth, size_t width, size_t stride, size_t height, uint8_t *data, size_t dataSize);
    ~SimpleImage();

    SimpleImage &operator=(const SimpleImage &image);
    SimpleImage &operator=(SimpleImage &&image) noexcept;

    void allocData(Format format, Depth depth, size_t width, size_t height);
    void freeData();
    void copy(Format format, Depth depth, size_t width, size_t stride, size_t height, uint8_t *data, size_t dataSize);

    Format format() const{return m_format;}
    Depth depth() const{return m_depth;}

    size_t width() const{return m_width;}
    size_t height() const{return m_height;}
    size_t stride() const{return m_stride;}

    uint8_t *data(){return m_data.data();}
    const uint8_t *data() const{return m_data.data();}
    size_t dataSize() const{return m_data.size();}

private:
    Format m_format;
    Depth m_depth;

    size_t m_width;
    size_t m_height;
    size_t m_stride;

    std::vector<uint8_t> m_data;
};

}//namespace imglib


//This is the functionality that you need to define for any custom image class you have to operate
//in this library. Make sure to define it in the imglib namespace as that is where the functions
//in this library will expect them. Make sure you declare (not just define) the functions in the 
//header, as there is a high likelyhood that the compilier will inline all these removing any function 
//call overhead.
namespace imglib
{

template<>
inline Format format<SimpleImage>(const SimpleImage &image) { return image.format(); }
template<>
inline Depth depth<SimpleImage>(const SimpleImage &image) { return image.depth(); }
template<>
inline Location location<SimpleImage>(const SimpleImage &image) { return Location::System; } //allways system for this guy

template<>
inline size_t width<SimpleImage>(const SimpleImage &image) { return image.width(); }
template<>
inline size_t height<SimpleImage>(const SimpleImage &image) { return image.height(); }
template<>
inline size_t stride<SimpleImage>(const SimpleImage &image) { return image.stride(); }

template<>
inline size_t nativeId<SimpleImage>(const SimpleImage &image) { return (size_t)image.data(); }
template<>
inline uint8_t *data<SimpleImage>(SimpleImage &image) { return image.data(); }
template<>
inline const uint8_t *data<SimpleImage>(const SimpleImage &image) { return image.data(); }
template<>
inline size_t dataSize<SimpleImage>(const SimpleImage &image) { return image.dataSize(); }

template<>
inline bool resize<SimpleImage>(SimpleImage &image, Format format, Depth depth, size_t width, size_t height)
{
    image.freeData();
    image.allocData(format, depth, width, height);
    return true;
}

}//namespace imglib

#endif //_imglib_simpleImage_h_