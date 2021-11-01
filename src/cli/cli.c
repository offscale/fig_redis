#include <stdlib.h>
#include <stdio.h>

#include <library.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <download_folder> <redis_version>", argv[0]);
        return EXIT_FAILURE;
    }
    return download_redis(argv[1], argv[2]);
}
