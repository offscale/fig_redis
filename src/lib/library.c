#include "library.h"

#include <stdio.h>

#include <acquire_amalgamation.h>

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
#define PATH_SEP "\\"
#else
#define PATH_SEP "/"
#endif

/* strlen("https://download.redis.io/releases/redis-00.00.00.tar.gz") == 56 */
#define MAX_REDIS_URL_LEN 56

int download_redis(const char* version) {
    const char *hash;
    int download_resp;
    char download_url[MAX_REDIS_URL_LEN] = "https://download.redis.io/releases/redis-"; /* 41 strlen */
    if (strcmp(version, "6.2.6") != 0) {
        fputs("TODO: Support more than version 6.2.6!", stderr);
        return EXIT_FAILURE;
    }
    const size_t version_len = strlen(version);
    {
        size_t i;
        for (i = 0; i < version_len; i++)
            download_url[41 + i] = version[i];
        i += 41;
        download_url[i++] = '.';
        download_url[i++] = 't';
        download_url[i++] = 'a';
        download_url[i++] = 'r';
        download_url[i++] = '.';
        download_url[i++] = 'g';
        download_url[i++] = 'z';
        download_url[i] = '\0';
    }
    hash = "5b2b8b7a50111ef395bf1c1d5be11e6e167ac018125055daa8b5c2317ae131ab";
    download_resp = download(download_url, LIBACQUIRE_SHA256,
                                       hash, "/tmp",
                                       false, 0, 0);
    if (download_resp != EXIT_SUCCESS) {
        fprintf(stderr, "download_resp: %d\n", download_resp);
        return download_resp;
    } else if (!sha256("/tmp/redis-6.2.6.tar.gz", hash)) {
        fputs("Checksum didn't match", stderr);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
