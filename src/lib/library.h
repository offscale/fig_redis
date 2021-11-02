#ifndef OS_REDIS_LIBRARY_H
#define OS_REDIS_LIBRARY_H

#include <string.h>

#include <acquire.h>

#include "version_redis_export.h"

/* strlen("3.2.0000-preview") */
#define MAX_REDIS_VERSION 16
/* strlen("redis-3.2.0000-preview.tar.gz") */
#define MAX_REDIS_FILENAME 29
/* #("https://github.com/microsoftarchive/redis/releases/download/win-3.2.0000-preview/Redis-x64-3.2.0000-preview.zip")
 * == 111 */
#define MAX_REDIS_URL_LEN 56
#define MAX_REDIS_HASH 65

struct RedisVersion {
    char version[MAX_REDIS_VERSION];
    char filename[MAX_REDIS_FILENAME];
    char hash[MAX_REDIS_HASH];
    char url[MAX_REDIS_URL_LEN];
    enum Checksum checksum;
    int status;
};

extern VERSION_REDIS_EXPORT int download_redis(const char *, const char*);

extern VERSION_REDIS_EXPORT void get_version_from_filename(const char filename[MAX_REDIS_FILENAME],
                                                           char *version[MAX_REDIS_FILENAME]) {
        size_t i, j, penultimate_dot=0, last_dot=0;
        for(i= strlen((const char *) filename) - 1, j=0; i != 0; i--, j++) {
            switch (filename[i]) {
                case '.':
                    if (penultimate_dot == 0)
                        penultimate_dot = i;
                    else last_dot = i;
                default:
                    *version[j] = filename[i];
            }
        }
        *version[j+1] = '\0';
}

#endif /* OS_REDIS_LIBRARY_H */
