#ifndef _imglib_interpolation_h_
#define _imglib_interpolation_h_

namespace imglib
{

enum Interpolation
{
    NearestNeighbor,
    BiLinear,
    BiCubic,
    Lanczos,
    SuperSample,
    Gradient
};

}//namespace imglib

#endif//_imglib_interpolation_h_
