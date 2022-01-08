#ifndef _imglib_format_type_h_
#define _imglib_format_type_h_

#include "imglib/image.h"

namespace imglib
{

//Format types to be used in templates for specialization

struct Format_Binary
{
    static constexpr Format value=Format::Binary;
    static constexpr size_t channels=1;
};

struct Format_GreyScale
{
    static constexpr Format value=Format::GreyScale;
    static constexpr size_t channels=2;
};

struct Format_RA
{
    static constexpr Format value=Format::RA;
    static constexpr size_t channels=2;
};

struct Format_RGB
{
    static constexpr Format value=Format::RGB;
    static constexpr size_t channels=3;
};

struct Format_RGBA
{
    static constexpr Format value=Format::RGBA;
    static constexpr size_t channels=4;
};

struct Format_RGBP
{
    static constexpr Format value=Format::RGBP;
    static constexpr size_t channels=3;
};

struct Format_RGBAP
{
    static constexpr Format value=Format::RGBAP;
    static constexpr size_t channels=4;
};

struct Format_BGR
{
    static constexpr Format value=Format::BGR;
    static constexpr size_t channels=3;
};

struct Format_BGRA
{
    static constexpr Format value=Format::BGRA;
    static constexpr size_t channels=4;
};

struct Format_BGRP
{
    static constexpr Format value=Format::BGRP;
    static constexpr size_t channels=3;
};

struct Format_BGRAP
{
    static constexpr Format value=Format::BGRAP;
    static constexpr size_t channels=4;
};

}//namespace imglib

#endif//_imglib_format_type_h_