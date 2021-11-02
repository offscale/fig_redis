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

get_libacquire_header("${CMAKE_BINARY_DIR}/src/acquire.h")

# Either the library downloads this file, CMake does, or we check the CMake version first,
# if desired version isn't available download new README.
# Security issues throughout because checksums must be verified, and we can't verify checksum of this:
file(DOWNLOAD "https://raw.githubusercontent.com/redis/redis-hashes/master/README"
        "${CMAKE_BINARY_DIR}/redis_versions")

function (get_releases org repo parent_struct)
    set(json_file "${CMAKE_BINARY_DIR}/${org}_releases.json")
    set(struct "")

    if (EXISTS "${header_file}")
        return()
    elseif (NOT EXISTS "${json_file}")
        file(DOWNLOAD "https://api.github.com/repos/${org}/${repo}/releases" "${json_file}"
                HTTPHEADER "Accept: application/vnd.github.v3+json"
                )
    endif ()
    file(READ "${json_file}" json_contents)
    string(LENGTH "${json_contents}" n)
    if (n EQUAL 0)
        message(FATAL_ERROR "Unable to download")
    endif ()

    string(JSON json_contents_n
            LENGTH "${json_contents}")
    math(EXPR json_contents_n "${json_contents_n} - 1")

    foreach (i RANGE 0 "${json_contents_n}")

        string(JSON type TYPE "${json_contents}" "${i}")
        if (NOT type STREQUAL "OBJECT")
            continue ()
        endif ()

        string(JSON type TYPE "${json_contents}" "${i}" "assets")
        if (NOT type STREQUAL "ARRAY")
            continue ()
        endif ()

        string(JSON assets_json
                GET "${json_contents}" "${i}" "assets")

        string(JSON assets_json_n
                LENGTH "${assets_json}")
        math(EXPR assets_json_n "${assets_json_n} - 1")

        foreach (j RANGE 0 "${assets_json_n}")
            string(JSON asset_json
                    GET "${assets_json}" "${j}")

            string(JSON type TYPE "${asset_json}")
            if (NOT type STREQUAL "OBJECT")
                continue ()
            endif ()

            string(JSON content_type
                    GET "${asset_json}" "content_type")
            string(JSON name
                    GET "${asset_json}" "name")

            if (content_type STREQUAL "application/zip" AND NOT EXISTS "${org}/${name}")
                string(JSON browser_download_url
                        GET "${asset_json}" "browser_download_url"
                        )
                set(dl_to "${CMAKE_BINARY_DIR}/${org}/${name}")
                file(DOWNLOAD "${browser_download_url}" "${dl_to}")
                file(READ "${dl_to}" contents)
                string(SHA256 contents_sha256 "${contents}")
                string(APPEND struct "\n\t{ \"${name}\", \"${contents_sha256}\", \"${browser_download_url}\","
                        "LIBACQUIRE_SHA256, EXIT_SUCCESS },")
            endif ()
        endforeach ()
    endforeach ()
    set(${parent_struct} "${struct}" PARENT_SCOPE)
endfunction (get_releases)


#if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
set(struct_fields "")
get_releases("microsoftarchive" "redis" struct_fields)

# Remove last ","
string(SUBSTRING "${struct_fields}" "0" "-1" struct_fields)

file(WRITE "windows_redis_versions.h"
        "static struct RedisVersion REDIS_VERSIONS[] = {"
        "${struct_fields}"
        "\n};")


#endif ()

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
        "${PROJECT_BINARY_DIR}/src/acquire.h"
        "${_export_file}"
        "${Header_Files}"
        DESTINATION
        "include")
install(TARGETS ${installable_libs}
        DESTINATION "bin"
        EXPORT "${LIBRARY_NAME}Targets")
