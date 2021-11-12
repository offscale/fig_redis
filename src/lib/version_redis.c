#include <stdio.h>
#include <errno.h>

#define LIBACQUIRE_IMPLEMENTATION
#include <acquire.h>
#undef LIBACQUIRE_IMPLEMENTATION

#include "version_redis.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows_redis_versions.h>
#else
#include <redis_versions.h>
#include <ctype.h>

#endif

struct RedisVersion get_version_info(const char *redis_versions_filepath, const char *version) {
    FILE * fh;
    char archive_name[MAX_REDIS_FILENAME];
    struct RedisVersion redisVersion;
    redisVersion.status = EXIT_FAILURE;

    snprintf(archive_name, MAX_REDIS_FILENAME,
             "redis-%s.tar.gz", version);

    fh = fopen(redis_versions_filepath, "r");
    if (fh == NULL) {
        fprintf(stderr, "fopen(\"%s\", \"r\") == NULL\n", redis_versions_filepath);
        redisVersion.status = ENOENT;
        return redisVersion;
    }
#define MAX_LEN 256

    char line[MAX_LEN], column[MAX_REDIS_HASH];
    const char *columns[5];
    while (fgets(line, MAX_LEN - 1, fh)) {
        line[strcspn(line, "\n")] = 0;
        switch (line[0]) {
            case '\0':
            case '#':
                continue;
        }

        {
            size_t i, j, k;
            for (i = 0, j = 0, k = 0; line[i] != '\0'; i++)
                switch (line[i]) {
                    case ' ':
                        column[j] = '\0';
                        columns[k++] = strdup(column);
                        j = 0;
                        break;
                    default:
                        column[j++] = line[i];
                }
            column[j] = '\0';
            columns[k] = strdup(column);
        }
        if (strcmp(archive_name, columns[1]) == 0) {
            redisVersion.status = EXIT_SUCCESS;
            char *version_arr_ptr = redisVersion.version;
            get_version_from_filename(redisVersion.filename, &version_arr_ptr);
            strncpy(redisVersion.filename, columns[1], MAX_REDIS_FILENAME);
            strncpy(redisVersion.hash, columns[3], MAX_REDIS_HASH);
            strncpy(redisVersion.url, columns[4], MAX_REDIS_URL_LEN);
            redisVersion.checksum = strcmp(columns[2], "sha1") == 0 ? LIBACQUIRE_UNSUPPORTED_CHECKSUM : LIBACQUIRE_SHA256;
            fclose(fh);
            return redisVersion;
        }
    }
#undef MAX_LEN

    fclose(fh);
    fprintf(stderr, "Redis version %s not found in \"%s\"\n", version, redis_versions_filepath);
    return redisVersion;
}

int download_redis(const char *download_folder, const char* version) {
    char full_local_fname[NAME_MAX + 1], full_local_fname_redis_versions[NAME_MAX + 1];
    int return_code;

    snprintf(full_local_fname_redis_versions, NAME_MAX + 1,
             "%s"PATH_SEP"%s", download_folder, "README");

    if (!is_file(full_local_fname_redis_versions)) {
        /* Security advisory: because one can't check the checksum of this file—
         * without constantly updating the binary—the contents can't really be trusted.
         *
         * Recommend downloading this file out-of-band, verifying, then using it.
         *
         * Otherwise, hashes and download links become only a verification and not a
         * security measure.
         *
         * Note however for versions released before this binary was released, it is
         * actually embedded within the binary so if there are issues others will
         * (hopefully!) find them, as everyone will have the same embed.
         * */
        download("https://raw.githubusercontent.com/redis/redis-hashes/master/README",
                 LIBACQUIRE_UNSUPPORTED_CHECKSUM, NULL, download_folder,
                 false, 0, 0);
    }

    {
        struct RedisVersion redisVersion = get_version_info(full_local_fname_redis_versions, version);

        if (redisVersion.status != EXIT_SUCCESS)
            return redisVersion.status;

        snprintf(full_local_fname, NAME_MAX + 1,
                 "%s"PATH_SEP"%s", download_folder, redisVersion.filename);

#ifdef VERSION_REDIS_DEBUG
        printf("redisVersion.url:\t\t\"%s\"\n"
               "redisVersion.checksum:\t%s\n"
               "redisVersion.hash:\t\t\"%s\"\n"
               "download_folder:\t\t\"%s\"\n",
               redisVersion.url,
               redisVersion.checksum == LIBACQUIRE_SHA256 ? "LIBACQUIRE_SHA256" : "LIBACQUIRE_UNSUPPORTED_CHECKSUM",
               redisVersion.hash,
               download_folder);
#endif

        return_code = download(redisVersion.url, redisVersion.checksum,
                               redisVersion.hash, download_folder,
                               false, 0, 0);
        if (return_code != EXIT_SUCCESS) {
            fprintf(stderr,
                    "`download(\"%s\", \"%s\", \"%s\", \"%s\", false, 0, 0)` returned %d\n",
                    redisVersion.url,
                    redisVersion.checksum == LIBACQUIRE_SHA256 ? "LIBACQUIRE_SHA256"
                                                               : "LIBACQUIRE_UNSUPPORTED_CHECKSUM",
                    redisVersion.hash,
                    download_folder,
                    return_code);
            return return_code;
        } else if (!sha256(full_local_fname, redisVersion.hash)) {
            fprintf(stderr, "%s hashes didn't match, checksum of \"%s\" is not:\n"
                            "\t%s\n",
                    redisVersion.checksum == LIBACQUIRE_SHA256 ? "SHA256" : "LIBACQUIRE_UNSUPPORTED_CHECKSUM",
                    full_local_fname, redisVersion.hash);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

extern void get_version_from_filename(const char filename[MAX_REDIS_FILENAME],
                                      char version[MAX_REDIS_VERSION]) {
    const char *extension = get_extension(filename);
    const size_t extension_n = strlen(extension);
    const size_t filename_n = strlen(filename);
    const size_t n = filename_n - extension_n;

    size_t i, j, k;
    bool munch;

    for(i=0, j=0, munch=false; i < n; i++)
        if (isdigit(filename[i]) || filename[i] == '.') {
            if (!munch) j = i, munch = true;
        } else if (munch) {
            k = i;
            if (k - j == 2) munch = false, j=0;
            else break;
        }

    strncpy(version, filename+j, filename_n-j-extension_n);
    version[filename_n-j-extension_n] = '\0';
}

int ls_remote(void) {
    unsigned short i;
    for(i=0; i<REDIS_VERSIONS_N; i++)
        puts(REDIS_VERSIONS[i].version);
    return EXIT_SUCCESS;
}
