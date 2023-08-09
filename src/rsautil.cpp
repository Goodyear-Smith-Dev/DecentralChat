#include <cstdlib>
#include <cassert>

#include "rsautil.hpp"

#include <cryptopp/base64.h>
#include <cryptopp/config_int.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/pkcspad.h>
#include <cryptopp/queue.h>
#include <cryptopp/sha.h>
#include <cryptopp/osrng.h>

namespace rsa = decentralchat::rsa;

using CryptoPP::byte;

// Generate public and private RSA keys for signing transactions
std::tuple<CryptoPP::InvertibleRSAFunction, RSA::PublicKey, RSA::PrivateKey> rsa::generateKeys() noexcept(false) {
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
CryptoPP::SecByteBlock rsa::sign(const std::string& message, const RSA::PrivateKey& privateKey) noexcept(false) {
	CryptoPP::AutoSeededRandomPool rng;

	CryptoPP::RSASSA_PKCS1v15_SHA_Signer signer(privateKey);

	size_t length = signer.MaxSignatureLength();
	CryptoPP::SecByteBlock signature(length);

	length = signer.SignMessage(rng, reinterpret_cast<const byte*>(message.c_str()), message.size(), signature.BytePtr());
	signature.resize(length);

	return signature;
}

// Verify a transaction using the sender's public key. Returns true if verification succeded, false otherwise.
bool rsa::verify(const std::string& message, const CryptoPP::SecByteBlock& signature, const RSA::PublicKey& publicKey) {
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::InvertibleRSAFunction params;
	CryptoPP::RSASSA_PKCS1v15_SHA_Verifier verifier(publicKey);

	return verifier.VerifyMessage(reinterpret_cast<const byte*>(message.c_str()), message.size(), signature.BytePtr(), signature.size());
}

CryptoPP::SecByteBlock rsa::encrypt(const std::string& input, const RSA::PublicKey& publicKey) noexcept(false) {
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::SecByteBlock plaintext(reinterpret_cast<const byte*>(input.c_str()), input.size());
	CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor(publicKey);
	std::size_t maxLen = encryptor.FixedMaxPlaintextLength();

	// Validate the RSA encryptor
	assert(maxLen > 0);
	assert(plaintext.size() <= maxLen);

	// Create a place to store the cypher text
	std::size_t cipherLen = encryptor.CiphertextLength(plaintext.size());
	assert(cipherLen > 0);
	CryptoPP::SecByteBlock ciphertext(cipherLen);

	// Encrypt the message using the public key
	encryptor.Encrypt(rng, plaintext, plaintext.size(), ciphertext);

	return ciphertext;
}

std::string rsa::decrypt(const CryptoPP::SecByteBlock& ciphertext, const RSA::PrivateKey& privateKey) noexcept(false) {
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::RSAES_OAEP_SHA_Decryptor decryptor(privateKey);
	std::size_t maxLen = decryptor.FixedCiphertextLength();

	// Validate the RSA decryptor
	assert(maxLen > 0);
	assert(ciphertext.size() <= maxLen);

	// Create a place to store the recovered text
	std::size_t plainTextLen = decryptor.MaxPlaintextLength(ciphertext.size());
	assert(plainTextLen > 0);
	CryptoPP::SecByteBlock recovered(plainTextLen);

	CryptoPP::DecodingResult result = decryptor.Decrypt(rng, ciphertext.BytePtr(), ciphertext.size(), recovered);

	// More sanity checks
	assert(result.isValidCoding);
	assert(result.messageLength <= decryptor.MaxPlaintextLength(ciphertext.size()));

	recovered.resize(result.messageLength); // Set the size of the recovered data

	// Return the recovered text as a string
	return std::string(reinterpret_cast<const char*>(recovered.BytePtr()), recovered.size());
}

void rsa::saveKey(const CryptoPP::RSAFunction& key, const std::string& filename, EncodingType encodingType) {
	CryptoPP::ByteQueue queue;
	key.Save(queue);

	CryptoPP::Filter* encoder;

	switch (encodingType) {
		case EncodingType::HEX: {
			encoder = new CryptoPP::HexEncoder;
			break;
		}

		case EncodingType::BASE64: {
			encoder = new CryptoPP::Base64Encoder;
			break;
		}

		default:
			break;
	}

	queue.CopyTo(*encoder);
	encoder->MessageEnd();

	CryptoPP::FileSink fileSink(filename.c_str());
	encoder->CopyTo(fileSink);
	fileSink.MessageEnd();

	delete encoder;
}

std::string rsa::keyToBase64(const CryptoPP::RSAFunction& key) {
	std::string base64Key;

	CryptoPP::Base64Encoder encoder(new CryptoPP::StringSink(base64Key));
    key.DEREncode(encoder);
	encoder.MessageEnd();

	return base64Key;
}

CryptoPP::RSAFunction rsa::keyFromBase64(const std::string& base64Key) {
	CryptoPP::RSAFunction key;

	CryptoPP::StringSource source(base64Key, true, new CryptoPP::Base64Decoder);
	key.BERDecode(source);
	source.MessageEnd();

	return key;
}