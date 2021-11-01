#include <stdlib.h>
#include <stdio.h>

#include <library.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <redis_version>", argv[0]);
        return EXIT_FAILURE;
    }
    return download_redis(argv[1]);
}
