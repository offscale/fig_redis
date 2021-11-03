Security
========

## 0

Checksums for redis versions are checked against the `REDIS_VERSIONS` array. This array is generated from the [cmake/modules/GenerateRedisVersions.cmake](cmake/modules/GenerateRedisVersions.cmake) which:

  0. Downloads [redis/redis-hashes](https://github.com/redis/redis-hashes/blob/master/README) for non-Windows and [microsoftarchive/redis](https://github.com/microsoftarchive/redis) & [tporadowski/redis](https://github.com/tporadowski/redis) repos for Windows;
  1. Parse out contents; on Windows repos download archives and generate their checksum;
  2. Emit struct arrays to [`redis_versions.h`](src/lib/redis_versions.h) & [`windows_redis_versions.h`](src/lib/windows_redis_versions.h)

However, on non-Windows when a version requested by user isn't found, https://raw.githubusercontent.com/redis/redis-hashes/master/README is used, which contains all the redis version; including associated: URL, checksum, and filename.
Because one can't check the checksum of this file—without constantly updating the binary—the contents can't really be trusted.

Recommend downloading this file out-of-band, verifying, then using it. Or adding a new issue to request re-run of CMake to get the latest version info without aforementioned security risk.

Relying on non-binary held hashes and download links means they become only a verification and not a security measure.
