#include <stdio.h>
#include <stdlib.h>

#include <version_redis.h>

int main(int argc, char **argv) {
    /* TODO: docopt impl */
    switch (argc) {
        case 2:
            if (strcmp(argv[1], "ls-remote") == 0)
                return redis_ls_remote();
            else
                goto default_label;
        case 4:
            return redis_download_extract(argv[1], argv[2], argv[3]);
        default_label:
        default:
            fprintf(stderr, "Usage: %s <download_folder> <redis_version> <prefix_dir>\n"
                            "Usage: %s ls-remote",
                            argv[0], argv[0]);
            return EXIT_FAILURE;
    }
}
