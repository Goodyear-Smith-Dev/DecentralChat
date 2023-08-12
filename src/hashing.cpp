#include "hashing.hpp"

namespace hashing = decentralchat::hashing;

// Hash some data
std::string hashing::SHA256(const std::string& input) {
	CryptoPP::SHA256 hash;
	std::string digest;

	CryptoPP::StringSource ssource(input, true,
		new CryptoPP::HashFilter(hash,
			new CryptoPP::HexEncoder(
				new CryptoPP::StringSink(digest)
			)
		)
	);

	return digest;
}