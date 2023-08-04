#include "rsautil.hpp"

#include <cryptopp/config_int.h> // Use CryptoPP::byte
#include <cryptopp/cryptlib.h> 	 // Use GenerateRandomWithKeySize
#include <cryptopp/sha.h>		 // Use the SHA256 class
#include <cryptopp/osrng.h> 	 // Use AutoSeededRandomPool
#include <cryptopp/pkcspad.h>	 // Use the PKCS1v15 struct

using CryptoPP::byte;

// Generate public and private RSA keys for signing transactions
std::tuple<CryptoPP::InvertibleRSAFunction, RSA::PublicKey, RSA::PrivateKey> rsautil::generateKeys() noexcept(false) {
	CryptoPP::AutoSeededRandomPool rng; // Random number generator
	CryptoPP::InvertibleRSAFunction params;
	constexpr int KEY_SIZE = 4096, VALIDATION_LEVEL = 3;

	params.GenerateRandomWithKeySize(rng, KEY_SIZE);

	// Generate an RSA public-private key pair
	RSA::PrivateKey privateKey(params);
	RSA::PublicKey publicKey(params);

	// Validate the private key
	if (!privateKey.Validate(rng, VALIDATION_LEVEL)) {
		throw std::runtime_error("Private key validation failed. Aborting.");
	}

	// Validate the private key
	if (!publicKey.Validate(rng, VALIDATION_LEVEL)) {
		throw std::runtime_error("Public key validation failed. Aborting.");
	}

	return std::make_tuple(params, publicKey, privateKey);
}

// Sign a transaction with the sender's private key
CryptoPP::SecByteBlock rsautil::sign(const std::string& message, const RSA::PrivateKey& privateKey) noexcept(false) {
	CryptoPP::AutoSeededRandomPool rng;

	CryptoPP::RSASSA_PKCS1v15_SHA_Signer signer(privateKey);

	size_t length = signer.MaxSignatureLength();
	CryptoPP::SecByteBlock signature(length);

	length = signer.SignMessage(rng, reinterpret_cast<const byte*>(message.c_str()), message.size(), signature.BytePtr());
	signature.resize(length);

	return signature;
}

// Verify a transaction using the sender's public key. Returns true if verification succeded, false otherwise.
bool rsautil::verify(const std::string& message, const CryptoPP::SecByteBlock& signature, const RSA::PublicKey& publicKey) {
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::InvertibleRSAFunction params;
	CryptoPP::RSASSA_PKCS1v15_SHA_Verifier verifier(publicKey);

	return verifier.VerifyMessage(reinterpret_cast<const byte*>(message.c_str()), message.size(), signature.BytePtr(), signature.size());
}