#include "imglib/cuda/copy.h"

#include <cuda_runtime.h>

namespace imglib
{
namespace cuda
{

bool copyImage(void *srcData, Location srcLocation, size_t srcStride, void *dstData, Location dstLocation, size_t dstStride, size_t widthInBytes, size_t height)
{
    cudaMemcpyKind kind;

    if(srcLocation == Location::Cuda)
    {
        if(dstLocation == Location::Cuda)
            kind=cudaMemcpyDeviceToDevice;
        else if(dstLocation == Location::System)
            kind=cudaMemcpyDeviceToHost;
        else
            return false;
    }
    else if(dstLocation == Location::Cuda)
    {
        if(srcLocation == Location::System)
            kind=cudaMemcpyHostToDevice;
        else
            return false;
    }

    auto error=cudaMemcpy2D(dstData, dstStride, srcData, srcStride, widthInBytes, height, kind);

    if(error != cudaSuccess)
        return false;

    return true;
}

}//namespace cuda
}//namespace imglib