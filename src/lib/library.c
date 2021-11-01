#include "library.h"

#include <stdio.h>

#include <acquire_amalgamation.h>

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
#define PATH_SEP "\\"
#else
#define PATH_SEP "/"
#endif

/* strlen("redis-00.00.00.tar.gz") */
#define MAX_REDIS_FILENAME 21
/* strlen("https://download.redis.io/releases/redis-00.00.00.tar.gz") == 56 */
#define MAX_REDIS_URL_LEN 56
#define MAX_REDIS_HASH 65

struct RedisVersion {
    char filename[MAX_REDIS_FILENAME];
    char hash[MAX_REDIS_HASH];
    char url[MAX_REDIS_URL_LEN];
    enum Checksum checksum;
    int status;
};

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
            strncpy(redisVersion.filename, columns[1], MAX_REDIS_FILENAME);
            strncpy(redisVersion.hash, columns[3], MAX_REDIS_HASH);
            strncpy(redisVersion.url, columns[4], MAX_REDIS_URL_LEN);
            redisVersion.checksum = strcmp(columns[2], "sha1") == 0 ? UNSUPPORTED : LIBACQUIRE_SHA256;
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
         * */
        download("https://raw.githubusercontent.com/redis/redis-hashes/master/README",
                 UNSUPPORTED, NULL, download_folder,
                 false, 0, 0);
    }

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
           redisVersion.checksum == LIBACQUIRE_SHA256 ? "LIBACQUIRE_SHA256" : "UNSUPPORTED",
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
                redisVersion.checksum == LIBACQUIRE_SHA256 ? "LIBACQUIRE_SHA256" : "UNSUPPORTED",
                redisVersion.hash,
                download_folder,
                return_code);
        return return_code;
    } else if (!sha256(full_local_fname, redisVersion.hash)) {
        fprintf(stderr, "%s hashes didn't match, checksum of \"%s\" is not:\n"
                        "\t%s\n",
                        redisVersion.checksum == LIBACQUIRE_SHA256 ? "SHA256" : "UNSUPPORTED",
                        full_local_fname, redisVersion.hash);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
