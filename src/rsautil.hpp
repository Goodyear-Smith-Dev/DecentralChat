#pragma once

// Link Crypto++ on windows
#include "osdetect.h"
#ifdef DC_PLATFORM_WINDOWS
	#pragma comment(lib, "cryptlib.lib")
#endif

#include <exception>
#include <string>
#include <tuple>

#include <cryptopp/cryptlib.h>
#include <cryptopp/rsa.h>
#include <cryptopp/secblock.h>

using CryptoPP::RSA;

namespace decentralchat::rsa {
	enum class EncodingType {HEX, BASE64};

	std::tuple<RSA::PublicKey, RSA::PrivateKey, CryptoPP::InvertibleRSAFunction> generateKeys() noexcept(false);

	CryptoPP::SecByteBlock sign(const std::string& message, const RSA::PrivateKey& privateKey) noexcept(false);
	bool verify(const std::string& message, const CryptoPP::SecByteBlock& signature, const RSA::PublicKey& publicKey);

	CryptoPP::SecByteBlock encrypt(const std::string& input, const RSA::PublicKey& publicKey) noexcept(false);
	std::string decrypt(const CryptoPP::SecByteBlock& ciphertext, const RSA::PrivateKey& privateKey) noexcept(false);

	void saveKey(const CryptoPP::RSAFunction& key, const std::string& filename, EncodingType encodingType = EncodingType::BASE64);

	std::string keyToBase64(const CryptoPP::RSAFunction& key);
	CryptoPP::InvertibleRSAFunction keyFromBase64(const std::string& base64Key);
}