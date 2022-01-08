cmake_minimum_required(VERSION 3.12)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/Hunter/HunterGate.cmake)

HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.267.tar.gz"
    SHA1 "9c5c7fa6e17c2ae15dd922184bc7c51235aaae70"
)

project(imglib)

#remove min and max macro declares from windows builds, causes issues with std::min/max
add_definitions(-DNOMINMAX)

option(IMGLIB_USE_COMPILETIME_TYPE "Turn on/off support for compile time type informat" OFF)
option(IMGLIB_USE_PNG "Turn on/off support for PNG" ON)
option(IMGLIB_USE_JPEG "Turn on/off support for Jpeg" ON)
option(IMGLIB_USE_EIGEN "Turn on/off support for eigen library" OFF)
option(IMGLIB_USE_OPENCL "Turn on/off support for OpenCl" OFF)
option(IMGLIB_USE_OPENCL_CPP "Turn on/off support for OpenCl CPP classes" OFF)
option(IMGLIB_USE_CUDA "Turn on/off support for CUDA" OFF)
option(IMGLIB_BUILD_TEST "Turns on test application" OFF)
set(IMGLIB_USE_FASTCV OFF) #used automatically if found

#build embed
include(ExternalProject)

ExternalProject_Add(embed
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/embed
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/embed
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release #just need the release version
    INSTALL_COMMAND "" #disable install, just use it from build dir
)
ExternalProject_Get_Property(embed BINARY_DIR)

set(EMBED_LOCATION ${BINARY_DIR}/embed)
add_executable(embed::embed IMPORTED)
set_target_properties(embed::embed PROPERTIES IMPORTED_LOCATION ${EMBED_LOCATION})
#complete embed project

set(imglib_sources
    imglib/bufferImage.h
    bufferImage.cpp
    imglib/colorConversion.h
    colorConversion.cpp
    imglib/convert.h
    convert.cpp
    imglib/copy.h
    imglib/crop.h
    crop.cpp
    imglib/image.h
    image.cpp
    imglib/imageIo.h
    imglib/interpolation.h
    imglib/match.h
    match.cpp
    imglib/math.h
    math.cpp
    imglib/ppm.h
    ppm.cpp
    imglib/quantize.h
    quantize.cpp
    imglib/scale.h
    scale.cpp
    imglib/simpleImage.h
    simpleImage.cpp
    imglib/transform.h
    transform.cpp
)

set(imglib_libraries "")
set(imglib_compile_options "")
set(imglib_compile_defines "")

#if(IMGLIB_USE_COMPILETIME_TYPE)
#    hunter_add_package(ctti)
#    find_package(ctti REQUIRED CONFIG)
#
#    set(imglib_libraries ${imglib_libraries} ctti)
#endif()

if(IMGLIB_USE_EIGEN)
    hunter_add_package(Eigen)
    find_package(Eigen3 REQUIRED)

    set(imglib_libraries ${imglib_libraries} Eigen3::Eigen)

    set(imglib_compile_defines ${imglib_compile_defines} -DIMGLIB_USE_EIGEN)
endif()

if(IMGLIB_USE_PNG)
    hunter_add_package(PNG)
    find_package(PNG CONFIG REQUIRED)

    set(imglib_sources ${imglib_sources}
        imglib/png.h
        png.cpp
    )

    set(imglib_libraries ${imglib_libraries} PNG::png)

    set(imglib_compile_defines ${imglib_compile_defines} -DIMGLIB_USE_PNG)
endif()

if(IMGLIB_USE_JPEG)
    hunter_add_package(Jpeg)
    find_package(JPEG CONFIG REQUIRED)

    set(imglib_sources ${imglib_sources}
        imglib/jpeg.h
        jpeg.cpp
    )

    set(imglib_libraries ${imglib_libraries} JPEG::jpeg)

    set(imglib_compile_defines ${imglib_compile_defines} -DIMGLIB_USE_JPEG)
endif()

if(IMGLIB_USE_OPENCL)
    hunter_add_package(OpenCL)
    find_package(OpenCL CONFIG REQUIRED)

    hunter_add_package(OpenCL-cpp)
    find_package(OpenCL-cpp CONFIG REQUIRED)

    set(imglib_sources ${imglib_sources}
        imglib/cl/colorConversion_cl.h
        cl/colorConversion_cl.cpp
        imglib/cl/kernels_cl.h
        cl/kernels_cl.cpp
        imglib/cl/openCLImage.h
        cl/openCLImage.cpp
        imglib/cl/openCLImageBuffer.h
        cl/openCLImageBuffer.cpp
        imglib/cl/operators_cl.h
        cl/operators_cl.cpp
        imglib/cl/transform_cl.h
        cl/transform_cl.cpp
        imglib/cl/utils_cl.h
        cl/utils_cl.cpp
    )

    set(imglib_libraries ${imglib_libraries} 
        OpenCL::OpenCL 
        OpenCL-cpp::OpenCL-cpp
    )

    include(EmbedFile)
    set(embedFiles
        kernels/colorConversion.cl
        kernels/operators.cl
        kernels/transform.cl
        kernels/utils.cl
    )
    embed_files(embededFiles "${embedFiles}" "imglib::cl")
endif()

if(IMGLIB_USE_CUDA)
    find_package(CUDAToolkit)

    enable_language(CUDA)

    set(imglib_sources ${imglib_sources}
        imglib/cuda/convert.h
        cuda/convert.cu
        imglib/cuda/copy.h
        cuda/copy.cpp
        imglib/cuda/simpleImage.h
        cuda/simpleImage.cpp
    )

    set(imglib_libraries ${imglib_libraries}
        CUDA::toolkit
        CUDA::cudart
    ) 
endif()

if(IMGLIB_USE_FASTCV)
    set(imglib_sources ${imglib_sources}
        imglib/fastcv/scale.h
        fastcv/scale.cpp
    )

    set(imglib_libraries ${imglib_libraries}
        FastCV::FastCV
    )

    set(imglib_compile_defines ${imglib_compile_defines} -DIMGLIB_USE_FASTCV)
endif()

add_library(imglib 
    ${imglib_sources}
    ${embededFiles}
)


#force embed to build
add_dependencies(imglib embed)

include(GenerateExportHeader)
generate_export_header(imglib)

target_link_libraries(imglib PUBLIC ${imglib_libraries})

target_include_directories(imglib PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(imglib PUBLIC ${CMAKE_CURRENT_BINARY_DIR}) #export files will be here
target_compile_options(imglib PUBLIC ${imglib_compile_options})

#link to file system
include(Filesystem)
set_target_filesystem(imglib)

if(IMGLIB_USE_COMPILETIME_TYPE)
    target_compile_definitions(imglib PUBLIC -DIMGLIB_USE_COMPILETIME_TYPE)

    #imglib uses ctti which defines foreach functions, if qt is ever used then its macros interfere
    target_compile_definitions(imglib PUBLIC -DQT_NO_FOREACH)
endif()

if(IMGLIB_USE_OPENCL)
    target_compile_definitions(imglib PUBLIC -DIMGLIB_OPENCL)
    if(IMGLIB_USE_OPENCL_CPP)
        target_compile_definitions(imglib PUBLIC -DIMGLIB_OPENCL_CPP)
    endif()

    get_target_property(OPENCL_BIN_DIR OpenCL::OpenCL IMPORTED_LOCATION_DEBUG)
    get_filename_component(OPENCL_BIN_DIR ${OPENCL_BIN_DIR} DIRECTORY)
endif()

if(IMGLIB_USE_CUDA)
    target_compile_definitions(imglib PUBLIC -DIMGLIB_CUDA)
    set_target_properties(imglib PROPERTIES 
#        POSITION_INDEPENDENT_CODE ON
#        CUDA_SEPARABLE_COMPILATION ON
        CUDA_RESOLVE_DEVICE_SYMBOLS ON
    )
endif()

target_compile_definitions(imglib PUBLIC ${imglib_compile_defines})

#build test application
if(IMGLIB_BUILD_TEST)
    set(imglib_benchmark_headers
    )
    
    set(imglib_benchmark_sources
        test/benchmark.cpp
    )

    if(IMGLIB_USE_OPENCL)
        set(imglib_benchmark_headers ${imglib_benchmark_headers}
            test/benchmarkOpenCL.h
        )

        set(imglib_benchmark_sources ${imglib_benchmark_sources}
            test/benchmarkOpenCL.cpp
        )
    endif()

    add_executable(imglibBenchmark
        ${imglib_benchmark_headers}
        ${imglib_benchmark_sources}
    )

    target_link_libraries(imglibBenchmark imglib gflags-static opencl_util)

    set_target_properties(imglibBenchmark PROPERTIES FOLDER "benchmarks")

    if(WIN32)
        foreach(CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES} )
            string( TOUPPER ${CONFIG_TYPE} CONFIG_TYPE_UPPER )
            set_target_properties(imglibBenchmark PROPERTIES LINK_FLAGS_${CONFIG_TYPE_UPPER} "/SUBSYSTEM:CONSOLE")
            set_target_properties(imglibBenchmark PROPERTIES COMPILE_DEFINITIONS_${CONFIG_TYPE_UPPER} "_CONSOLE")
        endforeach()
    endif()

    set(RUNTIME_LIBS "")

    if(IMGLIB_USE_OPENCL)
        set(RUNTIME_LIBS ${RUNTIME_LIBS} ${OPENCL_BIN_DIR})
    endif()

    #message(FATAL_ERROR "RUNTIME_LIBS: ${RUNTIME_LIBS}")
    create_target_launcher(imglibBenchmark
        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBS}
    )
endif()

