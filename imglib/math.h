#ifndef _imglib_math_h_
#define _imglib_math_h_

#include "imglib/image.h"
#include "imglib/interpolation.h"

#include <functional>
#include <algorithm>

namespace imglib
{

namespace details
{

uint8_t mean(Format format, Depth depth, size_t width, size_t height, size_t stride, 
    const uint8_t *data, size_t dataSize);

}//namespace details

template<typename _Image>
uint8_t mean(_Image &image)
{
    return details::mean(format(image), depth(image), width(image), height(image), stride(image), 
        data(image), dataSize(image));
}

#if(__cplusplus >= 201703L) //C++17
template<typename T, typename Compare>
constexpr const T &clamp(const T &v, const T &lo, const T &hi, Compare compare)
{
    return std::clamp(v, lo, hi, compare);
}
template<typename T>
constexpr const T &clamp(const T &v, const T &lo, const T &hi)
{
    return std::clamp(v, lo, hi);
}
#else
template<typename T, typename Compare>
constexpr const T &clamp(const T &v, const T &lo, const T &hi, Compare compare)
{
    return compare(v, lo) ? lo : compare(hi, v) ? hi : v;
}
template<typename T>
constexpr const T &clamp(const T &v, const T &lo, const T &hi)
{
    return imglib::clamp<T, std::less<T>>(v, lo, hi, std::less<T>{});
}
#endif//__cplusplus >= 201703L

}//namespace imglib

#endif//_imglib_math_h_
