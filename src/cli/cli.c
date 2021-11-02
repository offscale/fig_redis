#include <stdlib.h>
#include <stdio.h>

#include <library.h>

int main(int argc, char **argv) {
    /* TODO: docopt impl */
    if (argc == 2) {

    } else if (argc != 3) {
        fprintf(stderr, "Usage: %s <download_folder> <redis_version>\n"
                        "Usage: %s ls-remote", argv[0], argv[0]);
        return EXIT_FAILURE;
    }
    return download_redis(argv[1], argv[2]);
}
