include("release/CPackConfig.cmake")

set(CPACK_INSTALL_CMAKE_PROJECTS
        "debug;version_redis;ALL;/"
        "release;version_redis;ALL;/"
)
