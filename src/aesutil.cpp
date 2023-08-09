#include <cstdlib>

#include "aesutil.hpp"

#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/config_int.h>

namespace aes = decentralchat::aes;

using aes::AESData;
using aes::AESHexData;

AESData aes::encrypt(const std::string& message) noexcept(false) {
	CryptoPP::AutoSeededRandomPool rng;

	CryptoPP::SecByteBlock key(AES::DEFAULT_KEYLENGTH);
	CryptoPP::SecByteBlock initVec(AES::BLOCKSIZE);

	// Create random data blocks for the secret key and initialization vector
	rng.GenerateBlock(key, key.size());
	rng.GenerateBlock(initVec, initVec.size());

	// Encrypt the message using AES in CBC mode
	CryptoPP::CBC_Mode<AES>::Encryption encrypter;
	encrypter.SetKeyWithIV(key, key.size(), initVec);

	// Perform the encryption
	std::string ciphertext;
	CryptoPP::StringSource ssource(message, true,
		new CryptoPP::StreamTransformationFilter(encrypter,
			new CryptoPP::StringSink(ciphertext)
		)
	);

	return AESData{ciphertext, key, initVec};
}

std::string aes::decrypt(const AESData& aesData) noexcept(false) {
	return decrypt(aesData.ciphertext, aesData.key, aesData.iv);
}

std::string aes::decrypt(const std::string& cipher, const CryptoPP::SecByteBlock& key, const CryptoPP::SecByteBlock& initVec) noexcept(false) {
	CryptoPP::CBC_Mode<AES>::Decryption decrypter;
	decrypter.SetKeyWithIV(key, key.size(), initVec);

	// Perform the decryption
	std::string recovered;
	CryptoPP::StringSource ssource(cipher, true,
		new CryptoPP::StreamTransformationFilter(decrypter,
			new CryptoPP::StringSink(recovered)
		)
	);

	return recovered;
}

AESHexData aes::dataToHex(const AESData& aesData) {
	return dataToHex(aesData.ciphertext, aesData.key, aesData.iv);
}

AESHexData aes::dataToHex(const std::string& ciphertext, const CryptoPP::SecByteBlock& key, const CryptoPP::SecByteBlock& initVec) {
	AESHexData hexData;

	CryptoPP::StringSource ciphertextSource(ciphertext, true,
		new CryptoPP::HexEncoder(
			new CryptoPP::StringSink(hexData.ciphertext)
		)
	);

	CryptoPP::StringSource keySource(key.BytePtr(), key.size(), true,
		new CryptoPP::HexEncoder(
			new CryptoPP::StringSink(hexData.key)
		)
	);

	CryptoPP::StringSource ivSource(initVec.BytePtr(), initVec.size(), true,
		new CryptoPP::HexEncoder(
			new CryptoPP::StringSink(hexData.iv)
		)
	);

	return hexData;
}

AESData aes::dataFromHex(const AESHexData& hexData) {
	return dataFromHex(hexData.ciphertext, hexData.key, hexData.iv);
}

AESData aes::dataFromHex(const std::string& ciphertext, const std::string& key, const std::string& initVec) {
	AESData aesData;

	CryptoPP::StringSource ciphertextSource(ciphertext, true,
		new CryptoPP::HexDecoder(
			new CryptoPP::StringSink(aesData.ciphertext)
		)
	);

	std::string decodedKey;
	CryptoPP::StringSource keySource(key, true,
		new CryptoPP::HexDecoder(
			new CryptoPP::StringSink(decodedKey)
		)
	);
	aesData.key = CryptoPP::SecByteBlock(reinterpret_cast<const byte*>(decodedKey.c_str()), decodedKey.size());

	std::string decodedIV;
	CryptoPP::StringSource ivSource(initVec, true,
		new CryptoPP::HexDecoder(
			new CryptoPP::StringSink(decodedIV)
		)
	);
	aesData.iv = CryptoPP::SecByteBlock(reinterpret_cast<const byte*>(decodedIV.c_str()), decodedIV.size());

	return aesData;
}