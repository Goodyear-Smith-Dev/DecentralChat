#pragma once

#include <tuple>
#include <string>

#include <cryptopp/aes.h>
#include <cryptopp/secblock.h>

using CryptoPP::AES;
using CryptoPP::byte;

namespace decentralchat::aes {
	// Represent raw, binary AES data
	struct AESData {
		std::string ciphertext;
		CryptoPP::SecByteBlock key;
		CryptoPP::SecByteBlock iv;
	};

	// Represent hex-encoded AES data
	struct AESHexData {
		std::string ciphertext;
		std::string key;
		std::string iv;
	};

	AESData encrypt(const std::string& message) noexcept(false);

	std::string decrypt(const AESData& aesData) noexcept(false);
	std::string decrypt(const std::string& ciphertext, const CryptoPP::SecByteBlock& key, const CryptoPP::SecByteBlock& initVec) noexcept(false);

	AESHexData dataToHex(const AESData& aesData);
	AESHexData dataToHex(const std::string& ciphertext, const CryptoPP::SecByteBlock& key, const CryptoPP::SecByteBlock& initVec);

	AESData dataFromHex(const AESHexData& hexData);
	AESData dataFromHex(const std::string& ciphertext, const std::string& key, const std::string& initVec);
}