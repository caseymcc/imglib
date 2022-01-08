#ifndef _imglib_bufferImage_h_
#define _imglib_bufferImage_h_

#include "imglib/image.h"
#include "imglib_export.h"

#include <vector>

namespace imglib
{

/// A image class provided by the library for quick access to image stored in a buffer.
class IMGLIB_EXPORT BufferImage
{
public:
    BufferImage();
    BufferImage(Format format, Depth depth, size_t width, size_t height, uint8_t *data, size_t dataSize);
    BufferImage(Format format, Depth depth, size_t width, size_t stride, size_t height, uint8_t *data, size_t dataSize);
    ~BufferImage();

    void setBuffer(Format format, Depth depth, size_t width, size_t height, uint8_t *data, size_t dataSize);
    void setBuffer(Format format, Depth depth, size_t width, size_t stride, size_t height, uint8_t *data, size_t dataSize);

    Format format() const{return m_format;}
    Depth depth() const{return m_depth;}

    size_t width() const{return m_width;}
    size_t height() const{return m_height;}
    size_t stride() const{return m_stride;}

    uint8_t *data(){return m_data;}
    const uint8_t *data() const{return m_data;}
    size_t dataSize() const{return m_dataSize;}

private:
    Format m_format;
    Depth m_depth;

    size_t m_width;
    size_t m_height;
    size_t m_stride;

    uint8_t *m_data;
    size_t m_dataSize;
};

}//namespace imglib


namespace imglib
{

template<>
inline Format format<BufferImage>(const BufferImage &image) { return image.format(); }
template<>
inline Depth depth<BufferImage>(const BufferImage &image) { return image.depth(); }
template<>
inline Location location<BufferImage>(const BufferImage &image) { return Location::System; } //allways system for this guy

template<>
inline size_t width<BufferImage>(const BufferImage &image) { return image.width(); }
template<>
inline size_t height<BufferImage>(const BufferImage &image) { return image.height(); }
template<>
inline size_t stride<BufferImage>(const BufferImage &image) { return image.stride(); }

template<>
inline size_t nativeId<BufferImage>(const BufferImage &image) { return (size_t)image.data(); }
template<>
inline uint8_t *data<BufferImage>(BufferImage &image) { return image.data(); }
template<>
inline const uint8_t *data<BufferImage>(const BufferImage &image) { return image.data(); }
template<>
inline size_t dataSize<BufferImage>(const BufferImage &image) { return image.dataSize(); }

template<>
inline bool resize<BufferImage>(BufferImage &image, Format format, Depth depth, size_t width, size_t height)
{
    //wraps and existing buffer cannot change that buffer as it is not owned by the class
    return false;
}

}//namespace imglib

#endif //_imglib_bufferImage_h_