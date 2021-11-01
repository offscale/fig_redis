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

struct RedisVersion {
    const char *filename;
    const char *hash;
    const char *url;
    enum Checksum checksum;
} redisVersion;

int get_version_info(const char *version) {
    int code = EXIT_FAILURE;
    FILE * fh;
    char archive_name[MAX_REDIS_FILENAME];

    snprintf(archive_name, MAX_REDIS_FILENAME,
             "redis-%s.tar.gz", version);

    fh = fopen("./redis_versions", "r");
    if (fh == NULL) {
        fputs("fh == NULL", stderr);
        return ENOENT;
    }
#define MAX_LEN 256
#define MAX_STR 65

    char line[MAX_LEN], column[MAX_STR];
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
            redisVersion.filename = strdup(columns[1]);
            redisVersion.hash = columns[3];
            redisVersion.url = columns[4];
            redisVersion.checksum = strcmp(columns[2], "sha1") == 0 ? UNSUPPORTED : LIBACQUIRE_SHA256;
            code = EXIT_SUCCESS;
            goto fin;
        }
    }
#undef MAX_LEN
#undef MAX_STR
fin:
    fclose(fh);
    return code;
}

int download_redis(const char *download_folder, const char* version) {
    char full_local_fname[NAME_MAX + 1];
    int download_resp;
    int get_version_info_resp = get_version_info(version);
    if (get_version_info_resp != EXIT_SUCCESS)
        return get_version_info_resp;
    snprintf(full_local_fname, NAME_MAX + 1,
             "%s"PATH_SEP"%s", download_folder, redisVersion.filename);
    download_resp = download(redisVersion.url, redisVersion.checksum,
                             redisVersion.hash, download_folder,
                             false, 0, 0);
    if (download_resp != EXIT_SUCCESS) {
        fprintf(stderr, "download_resp: %d\n", download_resp);
        return download_resp;
    } else if (!sha256(full_local_fname, redisVersion.hash)) {
        fputs("Checksum didn't match", stderr);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
