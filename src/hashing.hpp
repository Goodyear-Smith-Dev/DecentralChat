#pragma once

// Link Crypto++ on windows
#include "osdetect.h"
#ifdef DC_PLATFORM_WINDOWS
	#pragma comment(lib, "cryptlib.lib")
#endif

#include <string>

#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

namespace decentralchat::hashing {
	std::string SHA256(const std::string& input);
}