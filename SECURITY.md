Security
========

## 0

Internally, https://raw.githubusercontent.com/redis/redis-hashes/master/README is used, which contains all the redis version; including associated: URL, checksum, and filename. Because one can't check the checksum of this file—without constantly updating the binary—the contents can't really be trusted.

Recommend downloading this file out-of-band, verifying, then using it.

Otherwise, hashes and download links become only a verification and not a
security measure.
