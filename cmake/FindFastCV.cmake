if(DEFINED ENV{FASTCV_ROOT})
    set(FASTCV_ROOT $ENV{FASTCV_ROOT})
endif()

if(NOT FASTCV_ROOT)
    message(FATAL_ERROR "FASTCV_ROOT needs to be set either through cmake var FASTCV_ROOT or env FASTCV_ROOT")
endif()

if(EXISTS "${FASTCV_ROOT}/inc/fastcv.h")
    file(STRINGS "${FASTCV_ROOT}/inc/fastcv.h" FastCV_VERSION_STRING REGEX "^@version [0-9]+\.[0-9]+\.[0-9]+.*$")

    string(REGEX REPLACE "^@version ([0-9]+\.[0-9]+\.[0-9])+.*$" "\\1" FastCV_VERSION "${FastCV_VERSION_STRING}")
endif()

set(FastCV_INCLUDE_DIR  "${FASTCV_ROOT}/inc")

if(ANDROID)
    if(CMAKE_ANDROID_ARCH STREQUAL "arm")
        set(FastCV_LIBRARY ${FASTCV_ROOT}/lib/Android/lib32/libfastcv.a)
    elseif(CMAKE_ANDROID_ARCH STREQUAL "arm64")
        set(FastCV_LIBRARY ${FASTCV_ROOT}/lib/Android/lib64/libfastcv.a)
    else()
        message(FATAL_ERROR "Unsupported ABI: ${ANDROID_ABI}, only \"armeabi-v7a\" or \"arm64-v8a\" suported")
        return()
    endif()
else()
    message(FATAL_ERROR "FastCV only supported on Android")
    return()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FastCV REQUIRED_VARS FastCV_LIBRARY FastCV_INCLUDE_DIR VERSION_VAR FastCV_VERSION_STRING)

if(NOT TARGET FastCV::FastCV)
    add_library(FastCV::FastCV UNKNOWN IMPORTED)
    set_target_properties(FastCV::FastCV PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${FastCV_INCLUDE_DIR}")
    set_target_properties(FastCV::FastCV PROPERTIES INTERFACE_LINK_LIBRARIES "log")
    set_property(TARGET FastCV::FastCV APPEND PROPERTY IMPORTED_LOCATION "${FastCV_LIBRARY}")
endif()
