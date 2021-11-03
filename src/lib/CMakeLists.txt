set(LIBRARY_NAME "redis_version")
set(libacquire_git "https://raw.githubusercontent.com/offscale/libacquire/master")

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

get_libacquire_header("${PROJECT_BINARY_DIR}/src/acquire.h")

include(../../cmake/modules/GenerateRedisVersions.cmake)

set(Header_Files "version_redis.h")

#if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

generate_windows_redis_versions("windows_redis_versions.h")
list(APPEND Header_Files "windows_redis_versions.h")

#else ()

generate_redis_versions("redis_versions.h")
list(APPEND Header_Files "redis_versions.h")

#endif ()


source_group("Header Files" FILES "${Header_Files}")

set(Source_Files "version_redis.c")
source_group("Source Files" FILES "${Source_Files}")

add_library("${LIBRARY_NAME}" "${LIBRARY_TYPE_FLAG}" "${Header_Files}" "${Source_Files}")

target_include_directories(
        "${LIBRARY_NAME}"
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:include>"
)

target_link_libraries("${LIBRARY_NAME}" PUBLIC "${LIBACQUIRE_LIBRARIES}")

if (NOT DEFINED TARGET_ARCH)
    set(TARGET_ARCH ${CMAKE_HOST_SYSTEM_PROCESSOR})
endif ()

configure_file(config.h.in "${PROJECT_BINARY_DIR}/src/config.h" @ONLY)

# install rules
include(GenerateExportHeader)
set(_export_file "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}_export.h")
generate_export_header("${LIBRARY_NAME}" EXPORT_FILE_NAME "${_export_file}")

set(installable_libs "${LIBRARY_NAME}" "${PROJECT_NAME}_compiler_flags")

if (TARGET "${DEPENDANT_LIBRARY}")
    list(APPEND installable_libs "${DEPENDANT_LIBRARY}")
endif ()
install(FILES
        "${PROJECT_BINARY_DIR}/src/acquire.h"
        "${PROJECT_BINARY_DIR}/src/config.h"
        "${_export_file}"
        "${Header_Files}"
        DESTINATION
        "include")
install(TARGETS ${installable_libs}
        DESTINATION "bin"
        EXPORT "${LIBRARY_NAME}Targets")
