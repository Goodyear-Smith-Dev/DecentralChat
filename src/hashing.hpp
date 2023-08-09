#pragma once

#include <string>

#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

namespace decentralchat::hashing {
	std::string SHA256(const std::string& input);
}