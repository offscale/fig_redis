version_redis
=============
[![License](https://img.shields.io/badge/license-Apache--2.0%20OR%20MIT-blue.svg)](https://opensource.org/licenses/Apache-2.0)
[![CI for Linux, Windows, macOS](https://github.com/offscale/version_redis/workflows/CI%20for%20Linux,%20Windows,%20macOS/badge.svg)](https://github.com/offscale/version_redis/actions)
[![C89](https://img.shields.io/badge/C-89-blue)](https://en.wikipedia.org/wiki/C89_(C_version))

[rvm](https://rvm.io)/[nvm](https://github.com/nvm-sh/nvm) style package manager for [redis](https://redis.io); written in [C89](https://en.wikipedia.org/wiki/C89_(C_version)).

## Internals

Works with [libacquire](https://github.com/offscale/libacquire), offscale's header-only library that links in with the HTTPS, cryptographic, and unarchiving APIs of the host OS (overridable with `-DUSE_LIBCURL`, `-DLIBFETCH`, etc.).

## Setup

### Dependencies

  - [CMake](https://cmake.org) (3.19 or later)
  - C compiler (any that work with CMake, and were released within the last 30 years)
  - Crypto, HTTPS, and unarchiving library (see [libacquire](https://github.com/offscale/libacquire) README for what to override, defaults to OS builtin API)

### Build

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

---

## License

Licensed under either of

- Apache License, Version 2.0 ([LICENSE-APACHE](LICENSE-APACHE) or <https://www.apache.org/licenses/LICENSE-2.0>)
- MIT license ([LICENSE-MIT](LICENSE-MIT) or <https://opensource.org/licenses/MIT>)

at your option.

### Contribution

Unless you explicitly state otherwise, any contribution intentionally submitted
for inclusion in the work by you, as defined in the Apache-2.0 license, shall be
dual licensed as above, without any additional terms or conditions.
