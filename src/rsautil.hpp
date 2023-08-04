#pragma once

#include <exception>
#include <string>
#include <tuple>

#include <cryptopp/rsa.h> 		 // Use Crypto++'s RSA functionality
#include <cryptopp/secblock.h> 	 // Use SecByteBlock

using CryptoPP::RSA;

namespace rsautil {
	std::tuple<CryptoPP::InvertibleRSAFunction, RSA::PublicKey, RSA::PrivateKey> generateKeys() noexcept(false);

	CryptoPP::SecByteBlock sign(const std::string& message, const RSA::PrivateKey& privateKey) noexcept(false);
	bool verify(const std::string& message, const CryptoPP::SecByteBlock& signature, const RSA::PublicKey& publicKey);
}