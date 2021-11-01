set(LIBRARY_NAME "${PROJECT_NAME}")
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

get_libacquire_header("${CMAKE_BINARY_DIR}/src/acquire_amalgamation.h")

# Either the library downloads this file, CMake does, or we check the CMake version first,
# if desired version isn't available download new README.
# Security issues throughout because checksums must be verified, and we can't verify checksum of this:
file(DOWNLOAD "https://raw.githubusercontent.com/redis/redis-hashes/master/README"
        "${CMAKE_BINARY_DIR}/redis_versions")

set(Header_Files "library.h")
source_group("Header Files" FILES "${Header_Files}")

set(Source_Files "library.c")
source_group("Source Files" FILES "${Source_Files}")

add_library("${LIBRARY_NAME}" "${LIBRARY_TYPE_FLAG}" "${Header_Files}" "${Source_Files}")

target_include_directories(
        "${LIBRARY_NAME}"
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src>"
        "$<INSTALL_INTERFACE:include>"
)

target_link_libraries("${LIBRARY_NAME}" PRIVATE "${LIBACQUIRE_LIBRARIES}")

# install rules
include(GenerateExportHeader)
set(_export_file "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}_export.h")
generate_export_header("${LIBRARY_NAME}" EXPORT_FILE_NAME "${_export_file}")

set(installable_libs "${LIBRARY_NAME}" "${PROJECT_NAME}_compiler_flags")

if (TARGET "${DEPENDANT_LIBRARY}")
    list(APPEND installable_libs "${DEPENDANT_LIBRARY}")
endif ()
install(FILES
        "${PROJECT_BINARY_DIR}/src/acquire_amalgamation.h"
        "${_export_file}"
        "${Header_Files}"
        DESTINATION
        "include")
install(TARGETS ${installable_libs}
        DESTINATION "bin"
        EXPORT "${LIBRARY_NAME}Targets")
