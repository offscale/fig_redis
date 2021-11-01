#include "library.h"

#include <stdio.h>

#include <acquire_amalgamation.h>

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
#define PATH_SEP "\\"
#else
#define PATH_SEP "/"
#endif

int download_redis(const char* version) {
    const char *hash = "5b2b8b7a50111ef395bf1c1d5be11e6e167ac018125055daa8b5c2317ae131ab";
    const int download_resp = download("https://download.redis.io/releases/redis-6.2.6.tar.gz", LIBACQUIRE_SHA256,
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
