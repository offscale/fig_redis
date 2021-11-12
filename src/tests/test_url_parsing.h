#ifndef VERSION_REDIS_TEST_URL_PARSING_H
#define VERSION_REDIS_TEST_URL_PARSING_H

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#include <stdbool.h>

#include <greatest.h>

#include <version_redis.h>


TEST x_test_version_parsing(void) {
    char version[MAX_REDIS_VERSION],
         filename[MAX_REDIS_FILENAME];

    strncpy(filename, "Redis-x64-3.2.0000-preview.zip\0", MAX_REDIS_FILENAME);
    get_version_from_filename(filename, version);
    ASSERT_FALSE(strcmp(version, "3.2.0000-preview") != 0);

    memset(version, 0, MAX_REDIS_VERSION);
    memset(filename, 0, MAX_REDIS_FILENAME);

    strncpy(filename, "redis-6.2.6.tar.gz\0", MAX_REDIS_FILENAME);
    get_version_from_filename(filename, version);
    ASSERT_FALSE(strcmp(version, "6.2.6") != 0);

    PASS();
}

/* Suites can group multiple tests with common setup. */
SUITE (url_parsing_suite) {
    RUN_TEST(x_test_version_parsing);
}

#endif /* VERSION_REDIS_TEST_URL_PARSING_H */
