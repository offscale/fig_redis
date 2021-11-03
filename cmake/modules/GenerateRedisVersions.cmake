set(header_prelude
        "#ifndef VERSION_REDIS_VERSIONS_H\n#define VERSION_REDIS_VERSIONS_H\n\n#include \"version_redis.h\"\n\nstatic struct RedisVersion REDIS_VERSIONS[] = {")
set(header_conclude
        "\n#endif /* VERSION_REDIS_VERSIONS_H */\n")

###################
# Windows variant #
###################

function (get_releases org repo parent_struct parent_number_of_lines)
    set(number_of_lines 0)
    set(json_file "${CMAKE_BINARY_DIR}/${org}_releases.json")

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
                set(dl_to "${CMAKE_BINARY_DIR}/downloads/${org}/${name}")
                if (NOT EXISTS "${dl_to}")
                    file(DOWNLOAD "${browser_download_url}" "${dl_to}")
                endif ()
                file(READ "${dl_to}" contents)
                string(SHA256 contents_sha256 "${contents}")

                string(SUBSTRING
                        "${name}"
                        "10" #  strlen("Redis-x64-")
                        "-1" # "${version_n}"
                        version)
                string(LENGTH "${version}" version_n)
                math(EXPR version_n "${version_n} - 4")  # .zip
                string(SUBSTRING
                        "${version}"
                        "0" # was one SUBSTRING rather than this and 3 lines above
                        "${version_n}"
                        version)

                string(APPEND struct "\n\t{\n"
                        "\t\t\"${version}\",\n"
                        "\t\t\"${name}\",\n"
                        "\t\t\"${contents_sha256}\",\n"
                        "\t\t\"${browser_download_url}\",\n"
                        "\t\tLIBACQUIRE_SHA256,\n"
                        "\t\tEXIT_SUCCESS\n"
                        "\t},")
                math(EXPR number_of_lines "${number_of_lines} + 1")
            endif ()
        endforeach ()
    endforeach ()
    set(${parent_struct} "${struct}" PARENT_SCOPE)

    set(${parent_number_of_lines} "${number_of_lines}" PARENT_SCOPE)
endfunction (get_releases)

function (generate_windows_redis_versions header_file)
    set(struct_fields "")
    set(total_number_of_lines 0)
    foreach (org "microsoftarchive" "tporadowski")
        set(number_of_lines 0)
        if (NOT EXISTS "${CMAKE_BINARY_DIR}/downloads/${org}")
            file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/downloads/${org}")
        endif ()
        set(struct_props "")
        get_releases("${org}" "redis" struct_props number_of_lines)
        math(EXPR total_number_of_lines "${total_number_of_lines} + ${number_of_lines}")
        string(APPEND struct_fields "${struct_props}")
    endforeach ()

    # Remove last ","
    string(LENGTH "${struct_fields}" struct_fields_n)
    math(EXPR struct_fields_n "${struct_fields_n} - 1")
    string(SUBSTRING "${struct_fields}" "0" "${struct_fields_n}" struct_fields)

    file(WRITE "${header_file}"
            "${header_prelude}"
            "${struct_fields}"
            "\n};\n\n"
            "static const unsigned short REDIS_VERSIONS_N = ${total_number_of_lines};\n"
            "${header_conclude}")
    message(STATUS "Generated \"${header_file}\" from GitHub API and local hashing")
endfunction ()

###################
# Linux variant #
###################

function (parse_out_version name version)
    string(LENGTH "${name}" _len)
    math(EXPR len "${_len} - 1")

    set(ver "")
    set(eat 0)
    foreach (char_index RANGE ${len})
        string(SUBSTRING "${name}" "${char_index}" "1" char)
        #string(LENGTH "${name}" line_len)
        string(TOLOWER "${char}" ch_low)
        string(TOUPPER "${char}" ch_up)
        if (eat OR "${ch_up}" STREQUAL "${ch_low}" AND NOT char STREQUAL "-")
            set(eat 1)
            string(APPEND ver "${char}")
        endif ()
    endforeach ()

    string(LENGTH "${ver}" ver_n)
    math(EXPR len "${ver_n} - 7") # - len(".tar.gz")
    string(SUBSTRING "${ver}" "0" "${len}" ver)

    set(${version} "${ver}" PARENT_SCOPE)
endfunction ()


function (parse_emit_versions in_fname out_fname)
    file(READ "${in_fname}" contents)
    string(TOUPPER "${name}" upper_name)
    set(new_contents "${header_prelude}\n")
    set(line "")
    set(skip_line 0)
    set(number_of_lines 0)
    string(LENGTH "${contents}" _len)
    math(EXPR len "${_len} - 1")

    function (clear_vars)
        set(filename "" PARENT_SCOPE)
        set(checksum "" PARENT_SCOPE)
        set(hash "" PARENT_SCOPE)
    endfunction (clear_vars)
    clear_vars()

    foreach(char_index RANGE ${len})
        string(SUBSTRING "${contents}" "${char_index}" "1" char)
        string(LENGTH "${line}" line_len)
        if (char STREQUAL "\n")
            if (NOT skip_line AND line_len)
                parse_out_version("${filename}" version)
                math(EXPR number_of_lines "${number_of_lines} + 1")
                string(APPEND new_contents
                        "\t{\n"
                        "\t\t\"${version}\",\n"
                        "\t\t\"${filename}\",\n"
                        "\t\t\"${hash}\",\n"
                        "\t\t\"${line}\",\n"
                        "\t\t${checksum},\n"
                        "\t\tEXIT_SUCCESS\n"
                        "\t},\n"
                        )
            endif ()
            set(skip_line 0)
            set(line "")
            clear_vars()
        elseif (char STREQUAL "#")
            set(skip_line 1)
        elseif (char STREQUAL " ")
            if (line_len)
                if (line STREQUAL "hash")
                elseif (filename STREQUAL "")
                    set(filename "${line}")
                elseif (checksum STREQUAL "")
                    if (line STREQUAL "sha256")
                        set(checksum "LIBACQUIRE_SHA256")
                    elseif (line STREQUAL "sha512")
                        set(checksum "LIBACQUIRE_SHA512")
                    else ()
                        set(checksum "UNSUPPORTED")
                    endif ()
                elseif (hash STREQUAL "")
                    set(hash "${line}")
                elseif (url STREQUAL "")
                    set(url "${line}")
                endif ()
                set(line "")
            endif (line_len)
        else ()
            string(APPEND line "${char}")
        endif ()
    endforeach()
    string(APPEND new_contents
            "};\n\n"
            "static const unsigned short REDIS_VERSIONS_N = ${number_of_lines};\n"
            "${header_conclude}")
    file(WRITE "${out_fname}" "${new_contents}")
    message(STATUS "Generated \"${out_fname}\" from \"${in_fname}\"")
endfunction()


function (generate_redis_versions header_file)
    # Either the library downloads this file, CMake does, or we check the CMake version first,
    # if desired version isn't available download new README.
    # Security issues throughout because checksums must be verified, and we can't verify checksum of this:
    file(DOWNLOAD "https://raw.githubusercontent.com/redis/redis-hashes/master/README"
            "${CMAKE_BINARY_DIR}/redis_versions")

    parse_emit_versions("${CMAKE_BINARY_DIR}/redis_versions" "${header_file}")
endfunction ()
