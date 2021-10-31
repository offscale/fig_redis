cmake_minimum_required(VERSION 3.20)
project(os_redis C)

set(CMAKE_C_STANDARD 90)

set(libacquire_git "https://raw.githubusercontent.com/offscale/libacquire/master")

include(CheckIncludeFiles)
foreach(cm "libacquire_get_header.cmake" "FindCurlCustom.cmake"
        "FindLibreSSL.cmake" "libacquire_helpers.cmake"
        "FindLibAcquire.cmake")
    if (NOT EXISTS "${CMAKE_BINARY_DIR}/cmake/${cm}")
        file(DOWNLOAD "${libacquire_git}/cmake/${cm}"
                "${CMAKE_BINARY_DIR}/cmake/${cm}")
    endif ()
    if (NOT cm STREQUAL "FindLibreSSL.cmake")
        include("${CMAKE_BINARY_DIR}/cmake/${cm}")
        if (cm STREQUAL "libacquire_helpers.cmake")
            set_crypto_lib()

            set_ssl_lib()

            set_http_https_lib()
        endif ()
    endif ()
endforeach()

get_libacquire_header("${CMAKE_BINARY_DIR}/src/acquire_amalgamation.h")

add_library(os_redis library.c)
target_include_directories(
        "os_redis"
        PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src>"
        "$<INSTALL_INTERFACE:include>"
)
