#include <stdlib.h>
#include <stdio.h>

#include <library.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows_redis_versions.h>
#else
#include <redis_versions.h>
#include <assert.h>

#endif

int main(int argc, char **argv) {
    /* TODO: docopt impl */
    if (argc == 2) {
        unsigned short i;
        assert(strcmp(argv[1], "ls-remote"));
        for(i=0; i<REDIS_VERSIONS_N; i++)
            puts(REDIS_VERSIONS[i].version);
        return EXIT_SUCCESS;
    } else if (argc == 3) {
        return download_redis(argv[1], argv[2]);
    } else {
        fprintf(stderr, "Usage: %s <download_folder> <redis_version>\n"
                        "Usage: %s ls-remote", argv[0], argv[0]);
        return EXIT_FAILURE;
    }
}
