#ifndef _imglib_boundingBox_h_
#define _imglib_boundingBox_h_

namespace imglib
{

namespace bbox
{

template<typename _BoundingBox>
struct ParameterType
{
    using Type=int;
};

template<typename _BoundingBox>
typename ParameterType<_BoundingBox>::Type left(const _BoundingBox &){return 0;}
template<typename _BoundingBox>
typename ParameterType<_BoundingBox>::Type right(const _BoundingBox &){return 0;}
template<typename _BoundingBox>
typename ParameterType<_BoundingBox>::Type top(const _BoundingBox &){return 0;}
template<typename _BoundingBox>
typename ParameterType<_BoundingBox>::Type bottom(const _BoundingBox &){return 0;}

}//namespace bbox
}//namespace imglib

#endif//_imglib_boundingBox_h_