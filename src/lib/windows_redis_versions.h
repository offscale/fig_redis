#ifndef VERSION_REDIS_VERSIONS_H
#define VERSION_REDIS_VERSIONS_H

#include "version_redis.h"

static struct RedisVersion REDIS_VERSIONS[] = {
	{
		"3.2.100",
		"Redis-x64-3.2.100.zip",
		"02ccf955a1b8f76fdbeb87c23fbe07c90e3b58a312306a52f244a850a2fc9033",
		"https://github.com/microsoftarchive/redis/releases/download/win-3.2.100/Redis-x64-3.2.100.zip",
		LIBACQUIRE_SHA256,
		EXIT_SUCCESS
	},
	{
		"3.0.504",
		"Redis-x64-3.0.504.zip",
		"4534581ac55ef6873cb759917df38d817414f183901c168a8ae3a97aab3770f2",
		"https://github.com/microsoftarchive/redis/releases/download/win-3.0.504/Redis-x64-3.0.504.zip",
		LIBACQUIRE_SHA256,
		EXIT_SUCCESS
	},
	{
		"3.0.503",
		"Redis-x64-3.0.503.zip",
		"bafe0a3801bc366e7e92837ae0ece118d727fb44a5ed4f61807a464d96fb8b58",
		"https://github.com/microsoftarchive/redis/releases/download/win-3.0.503/Redis-x64-3.0.503.zip",
		LIBACQUIRE_SHA256,
		EXIT_SUCCESS
	},
	{
		"2.8.2402",
		"Redis-x64-2.8.2402.zip",
		"ed03900a3927e23931333141f083dfaf2cfe36c5d24994fe15a0ba765f5aa996",
		"https://github.com/microsoftarchive/redis/releases/download/win-2.8.2402/Redis-x64-2.8.2402.zip",
		LIBACQUIRE_SHA256,
		EXIT_SUCCESS
	},
	{
		"3.0.502",
		"Redis-x64-3.0.502.zip",
		"29fc76cb620ccc24931d072738051062b64c2db804ecb3aec8ece7f66f010ee4",
		"https://github.com/microsoftarchive/redis/releases/download/win-3.0.502/Redis-x64-3.0.502.zip",
		LIBACQUIRE_SHA256,
		EXIT_SUCCESS
	},
	{
		"2.8.2401",
		"Redis-x64-2.8.2401.zip",
		"88835eb34861e440751a6cd74dcd2ea6857a016b4c53d4df1ef5448237852e8c",
		"https://github.com/microsoftarchive/redis/releases/download/win-2.8.2401/Redis-x64-2.8.2401.zip",
		LIBACQUIRE_SHA256,
		EXIT_SUCCESS
	},
	{
		"3.2.0000-preview",
		"Redis-x64-3.2.0000-preview.zip",
		"01cca8b1fbe81bf27762d9bae3725e21d85e099ea69bde5427f9f9bf326d3e9b",
		"https://github.com/microsoftarchive/redis/releases/download/win-3.2.0000-preview/Redis-x64-3.2.0000-preview.zip",
		LIBACQUIRE_SHA256,
		EXIT_SUCCESS
	},
	{
		"2.8.2400",
		"Redis-x64-2.8.2400.zip",
		"13bd7686c426a3433f4aefb2f0c693f2e9bba0098c53df8a6755d7759dd4ccf4",
		"https://github.com/microsoftarchive/redis/releases/download/win-2.8.2400/Redis-x64-2.8.2400.zip",
		LIBACQUIRE_SHA256,
		EXIT_SUCCESS
	},
	{
		"3.0.501",
		"Redis-x64-3.0.501.zip",
		"771b0651fad7f05f79b81ae85fd4db626209cbc169742f2ed4ca14be07c2b07d",
		"https://github.com/microsoftarchive/redis/releases/download/win-3.0.501/Redis-x64-3.0.501.zip",
		LIBACQUIRE_SHA256,
		EXIT_SUCCESS
	}
};

static const unsigned short REDIS_VERSIONS_N = 9;

#endif /* VERSION_REDIS_VERSIONS_H */
