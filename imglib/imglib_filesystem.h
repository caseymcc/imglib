#ifndef _imglib_imglib_filesystem_h_
#define _imglib_imglib_filesystem_h_

#if IMGLIB_USE_FILESYSTEM == IMGLIB_STD_FILESYSTEM
#include <filesystem>
#elif IMGLIB_USE_FILESYSTEM == IMGLIB_EXPERIMENTAL_FILESYSTEM
#include <experimental/filesystem>
#else
#include <boost/filesystem.hpp>
#endif//IMGLIB_USE_FILESYSTEM

namespace imglib
{

#if IMGLIB_USE_FILESYSTEM == IMGLIB_STD_FILESYSTEM
namespace fs=std::filesystem;
#elif IMGLIB_USE_FILESYSTEM == IMGLIB_EXPERIMENTAL_FILESYSTEM
namespace fs=std::experimental::filesystem;
#else
namespace fs=boost::filesystem;
#endif//IMGLIB_USE_FILESYSTEM

}

#endif//_imglib_imglib_filesystem_h_
