#ifndef _imglib_vec_h_
#define _imglib_vec_h_

namespace imglib
{

namespace vec
{

namespace traits
{

template<typename _Type>
struct Value
{
    using Type=void;
};

}//namespace traits

template<typename _Type>
typename traits::Value<_Type>::Type &x(_Type &vec)
{
    static_assert(sizeof(_Type) == -1, "You have not declared your type properly");
    return 0;
}

template<typename _Type>
typename traits::Value<_Type>::Type &y(_Type &vec)
{
    static_assert(sizeof(_Type) == -1, "You have not declared your type properly");
    return 0;
}

template<typename _Type>
typename traits::Value<_Type>::Type &z(_Type &vec)
{
    static_assert(sizeof(_Type) == -1, "You have not declared your type properly");
    return 0;
}

template<typename _Type>
typename traits::Value<_Type>::Type &w(_Type &vec)
{
	static_assert(sizeof(_Type) == -1, "You have not declared your type properly");
    return 0;
}

template<typename _Type>
typename traits::Value<_Type>::Type x(const _Type &vec)
{
    static_assert(sizeof(_Type) == -1, "You have not declared your type properly");
    return 0;
}

template<typename _Type>
typename traits::Value<_Type>::Type y(const _Type &vec)
{
    static_assert(sizeof(_Type) == -1, "You have not declared your type properly");
    return 0;
}

template<typename _Type>
typename traits::Value<_Type>::Type z(const _Type &vec)
{
    static_assert(sizeof(_Type) == -1, "You have not declared your type properly");
    return 0;
}

template<typename _Type>
typename traits::Value<_Type>::Type w(const _Type &vec)
{
    static_assert(sizeof(_Type) == -1, "You have not declared your type properly");
    return 0;
}

template<typename _Type>
typename traits::Value<_Type>::Type red(const _Type &vec)
{
    static_assert(sizeof(_Type) == -1, "You have not declared your type properly");
    return 0;
}

template<typename _Type>
typename traits::Value<_Type>::Type green(const _Type &vec)
{
    static_assert(sizeof(_Type) == -1, "You have not declared your type properly");
    return 0;
}

template<typename _Type>
typename traits::Value<_Type>::Type blue(const _Type &vec)
{
    static_assert(sizeof(_Type) == -1, "You have not declared your type properly");
    return 0;
}

template<typename _Type>
typename traits::Value<_Type>::Type alpha(const _Type &vec)
{
    static_assert(sizeof(_Type) == -1, "You have not declared your type properly");
    return 0;
}

template<typename _Type>
const typename traits::Value<_Type>::Type *data(const _Type &vec)
{
    static_assert(sizeof(_Type) == -1, "You have not declared your type properly");
    return 0;
}

template<typename _Type>
typename traits::Value<_Type>::Type *data(_Type &vec)
{
    static_assert(sizeof(_Type) == -1, "You have not declared your type properly");
    return 0;
}

template<typename _Type>
_Type normalize(const _Type &vec)
{
    static_assert(sizeof(_Type) == -1, "You have not declared your type properly");
	return _Type();
}

template<typename _Type>
typename traits::Value<_Type>::Type length(const _Type &vec)
{
    static_assert(sizeof(_Type) == -1, "You have not declared your type properly");
	return typename traits::Value<_Type>::Type();
}

}//namespace vec
}//namespace imglib

#endif//_imglib_vec_h_
