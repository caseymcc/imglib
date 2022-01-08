#ifndef _imglib_vec_glm_h_
#define _imglib_vec_glm_h_

#include "imglib/vec.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace imglib
{
namespace vec
{

namespace traits
{

template<>
struct Value<glm::ivec2>
{
	using Type=int;
};

template<>
struct Value<glm::vec2>
{
	using Type=float;
};

template<>
struct Value<glm::vec3>
{
	using Type=float;
};

template<>
struct Value<glm::vec4>
{
	using Type=float;
};

template<>
struct Value<glm::tvec4<unsigned char>>
{
	using Type=unsigned char;
};

}//namespace traits

//ivec def
template<>
inline int &x(glm::ivec2 &vec)
{
	return vec.x;
}

template<>
inline int &y(glm::ivec2 &vec)
{
	return vec.y;
}

template<>
inline int x(const glm::ivec2 &vec)
{
	return vec.x;
}

template<>
inline int y(const glm::ivec2 &vec)
{
	return vec.y;
}

template<>
inline float x(const glm::vec2 &vec)
{
    return vec.x;
}

template<>
inline float y(const glm::vec2 &vec)
{
    return vec.y;
}

template<>
inline float &x(glm::vec2 &vec)
{
    return vec.x;
}

template<>
inline float &y(glm::vec2 &vec)
{
    return vec.y;
}

template<>
inline float x(const glm::vec3 &vec)
{
    return vec.x;
}

template<>
inline float y(const glm::vec3 &vec)
{
    return vec.y;
}

template<>
inline float z(const glm::vec3 &vec)
{
    return vec.z;
}

template<>
inline float &x(glm::vec3 &vec)
{
    return vec.x;
}

template<>
inline float &y(glm::vec3 &vec)
{
    return vec.y;
}

template<>
inline float &z(glm::vec3 &vec)
{
    return vec.z;
}

template<>
inline float red(const glm::vec4 &vec)
{
    return vec.r*255;
}

template<>
inline float green(const glm::vec4 &vec)
{
    return vec.g*255;
}

template<>
inline float blue(const glm::vec4 &vec)
{
    return vec.b*255;
}

template<>
inline float alpha(const glm::vec4 &vec)
{
    return vec.a*255;
}

template<>
inline unsigned char red(const glm::tvec4<unsigned char> &vec)
{
    return vec.r;
}

template<>
inline unsigned char green(const glm::tvec4<unsigned char> &vec)
{
    return vec.g;
}

template<>
inline unsigned char blue(const glm::tvec4<unsigned char> &vec)
{
    return vec.b;
}

template<>
inline unsigned char alpha(const glm::tvec4<unsigned char> &vec)
{
    return vec.a;
}

template<>
inline const float *data(const glm::vec2 &vec)
{
    return glm::value_ptr(vec);
}

template<>
inline const float *data(const glm::vec3 &vec)
{
    return glm::value_ptr(vec);
}

template<>
inline const float *data(const glm::vec4 &vec)
{
    return glm::value_ptr(vec);
}

template<>
inline const unsigned char *data(const glm::tvec4<unsigned char> &vec)
{
    return glm::value_ptr(vec);
}

template<>
inline float *data(glm::vec2 &vec)
{
    return glm::value_ptr(vec);
}

template<>
inline float *data(glm::vec3 &vec)
{
    return glm::value_ptr(vec);
}

template<>
inline float *data(glm::vec4 &vec)
{
    return glm::value_ptr(vec);
}

template<>
inline unsigned char *data(glm::tvec4<unsigned char> &vec)
{
    return glm::value_ptr(vec);
}

template<>
inline glm::vec2 normalize(const glm::vec2 &vec)
{
    return glm::normalize(vec);
}

template<>
inline glm::vec3 normalize(const glm::vec3 &vec)
{
    return glm::normalize(vec);
}

template<>
inline glm::vec4 normalize(const glm::vec4 &vec)
{
    return glm::normalize(vec);
}

template<>
inline float length(const glm::vec2 &vec)
{
    return glm::length(vec);
}

template<>
inline float length(const glm::vec3 &vec)
{
    return glm::length(vec);
}

template<>
inline float length(const glm::vec4 &vec)
{
    return glm::length(vec);
}

}//namespace vec
}//namespace imglib

#endif//_imglib_vec_glm_h_