#ifndef VERSION_REDIS_VERSION_REDIS_H
#define VERSION_REDIS_VERSION_REDIS_H

#include <string.h>

#include "config.h"

#include <acquire.h>

#include "redis_version_export.h"

/* strlen("3.2.0000-preview") == 16 */
#define MAX_REDIS_VERSION 17
/* strlen("Redis-x64-3.2.0000-preview.zip") == 30 */
#define MAX_REDIS_FILENAME 31
/* #("https://github.com/microsoftarchive/redis/releases/download/win-3.2.0000-preview/Redis-x64-3.2.0000-preview.zip")
 * == 111 */
#define MAX_REDIS_URL_LEN 112
#define MAX_REDIS_HASH 65

struct RedisVersion {
    char version[MAX_REDIS_VERSION];
    char filename[MAX_REDIS_FILENAME];
    char hash[MAX_REDIS_HASH];
    char url[MAX_REDIS_URL_LEN];
    enum Checksum checksum;
    int status;
};

extern REDIS_VERSION_EXPORT int download_redis(const char *, const char*);

extern REDIS_VERSION_EXPORT void get_version_from_filename(const char filename[MAX_REDIS_FILENAME],
                                                           char version[MAX_REDIS_VERSION]);

extern REDIS_VERSION_EXPORT int ls_remote(void);

#endif /* VERSION_REDIS_VERSION_REDIS_H */
