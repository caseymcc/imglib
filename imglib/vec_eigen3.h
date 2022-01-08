#ifndef _imglib_vec_eigen3_h_
#define _imglib_vec_eigen3_h_

#include <Eigen/Core>

namespace imglib
{

namespace traits
{

template<>
struct Value<Eigen>
{
	using Type=int;
};

}//namespace traits
}//namespace imglib

#endif//_imglib_vec_eigen3_h_