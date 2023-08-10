## Features

* RSA
  * Generate keys (4096 bits/512 bytes)
  * Signing/verifying messages
  * Encrypting/decrypting messages
  * Save keys to the filesystem (encoded has hex or base64)
  * Encode keys to a base64 `std::string`
  * Decode keys from base64 to `CryptoPP::RSAFunction`
* AES
  * Encryption/Decryption using AES in CBC (Cipher Block Chaining) mode
  * Convert AES cipher text, keys, and IVs to and from hex
  * Support storing and operating on AES data in structs (`AESData`/`AESHexData`)
* SHA-256
  * Accept plaintext as a string, return a hex-encoded digest

## Usage

### RSA

#### Header file

```cpp
#include "rsautil.hpp" // Chosen to avoid similarity with Crypto++'s rsa.h
```

#### Generating keys

Keys are returned in a tuple containing three things (in this order):

* The public key
* The private key
* The key parameters from Crypto++ that can be used to generate more copies of this specific key pair

When calling this fucntion, keys can be extracted from the tuple in one of two ways:

1. Using `std::tie`

    ```cpp
    // CryptoPP::PublicKey and CryptoPP::PrivateKey are abstract classes,
    // so you have to use CryptoPP::RSAFunction to assign to them.
    CryptoPP::RSAFunction publicKey, privateKey;
    CryptoPP::InvertibleRSAFunction params;
    std::tie(publicKey, privateKey, params) = decentralchat::rsa::generateKeys();
    ```

2. Using a structured binding (C++17 and later)

    ```cpp
    // The rvalue reference isn't required, but might make sense here
    auto&& [publicKey, privateKey, params] = decentralchat::rsa::generateKeys();
    ```

NOTE: This function can throw exceptions.

#### Signing and verifying a message

Signing and verifying uses `decentralchat::rsa::sign` and `decentralchat::rsa::verify`, respectively.
Signing returns a `CryptoPP::SecByteBlock`, while verifying returns a `bool` (`true` if verified, `false` otherwise).

```cpp
std::string message = "Hello, World!";

// Sign the message
auto signature = decentralchat::rsa::sign(message, privateKey);

// Verify
bool verified = decentralchat::rsa::verify(message, signature, publicKey);

// Do something now that the signature is verified
if (verified) {
  ...
}
```

NOTE: Both functions can throw exceptions.

#### Encrypting/decrypting a message

Encrypting and decrypting uses `decentralchat::rsa::encrypt` and `decentralchat::rsa::decrypt`, respectively.
Encrypting returns a `CryptoPP::SecByteBlock`, and decrypting returns a `std::string`, which should be the original plaintext before encrypting.

```cpp
std::string plaintext = "Hello, World!";

// Encrypt
auto ciphertext = decentralchat::rsa::encrypt(plaintext, publicKey);

// Decrypt
std::string recovered = decentralchat::rsa::decrypt(ciphertext, privateKey);

assert(plaintext == recovered) // If this fails, there's a problem
```

NOTE: Both functions can throw exceptions.

#### Saving keys to the filesystem

Keys can be saved using `decentralchat::rsa::saveKeys`. This function accepts either a public or private key, a filename, and an encoding type (base64 or hex).
If no encoding type is specfied, base64 is assumed.

```cpp
using decentralchat::rsa::EncodingType;

// Save keys as base64
decentralchat::rsa::saveKey(publicKey, "pubkey.key", EncodingType::BASE64);
decentralchat::rsa::saveKey(privateKey, "privkey.key", EncodingType::BASE64);

// Save keys as base64 (assumed by default)
decentralchat::rsa::saveKey(publicKey, "pubkey.key");
decentralchat::rsa::saveKey(privateKey, "privkey.key");

// Save keys as hex
decentralchat::rsa::saveKey(publicKey, "pubkey.key", EncodingType::HEX);
decentralchat::rsa::saveKey(privateKey, "privkey.key", EncodingType::HEX);
```

#### Encoding/decoding keys

In some situations, you might want to encode the keys without saving them.
This can be done using `decentralchat::rsa::keyToBase64` and `decentralchat::rsa::keyFromBase64`.
Encoding accepts any key and returns a `std::string`, while decoding accepts an encoded `std::string` and returns a `CryptoPP::RSAFunction`.

```cpp
std::string encoded = decentralchat::rsa::keyToBase64(publicKey);
CryptoPP::RSAFunction decoded = decentralchat::rsa::keyFromBase64(encoded);
```

### AES

#### Header file

```cpp
#include "aesutil.hpp" // Chosen to avoid similarity with Crypto++'s aes.h
```

#### Storing AES data with structs

When a message is encrypted using AES, you need three things to decrypt the ciphertext:

1. The ciphertext itself
2. The AES key
3. The initialization vector (IV)

To make it easier to pass these three things around, `aesutil` provides two structs that group AES data:

1. `AESData`, which stores raw, non-encoded AES data
2. `AESHexData`, which stores hex-encoded AES data

If prefered or needed, you can still operate on the data separatly (i.e., not using the struct), but `aesutil` functions always return encrypted data in structs to avoid overuse of tuples.

If you need to extract values form the struct, you can pass the struct fields (`AESData::ciphertext`, `AESData::key`, `AESData::iv`) or extract them using a structured binding. Note that `AESHexData` uses the same field names.

#### Encrypting/decrypting a message

Encrypting and decrypting uses `decentralchat::aes::encrypt` and `decentralchat::aes::decrypt`, respectively.
Encrypting returns `aes::AESData`, and decrypting returns a `std::string`, which should be the original plaintext before encrypting.

```cpp
using decentralchat::aes::AESData;

std::string plaintext = "Hello, World!";

// Encrypt the message
AESData aesData = decentralchat::aes::encrypt(plaintext);

// Decrypt the message using the struct
std::string recovered = decentralchat::aes::decrypt(aesData);

// Decrypt the message using individual fields
std::string recovered = decentralchat::aes::decrypt(aesData.ciphertex, aesData.key, aesData.iv);

assert(plaintext == recovered); // If this fails, there's a problem
```

NOTE: These functions can throw exceptions.

#### Encoding/decoding AES data

Bianry AES data can be encoded into hex using `decentralchat::aes::dataToHex` and decoded using `decentralchat::aes::dataFromHex`.
Both functions can either operate on structs or separated data. Note that `dataToHex` accepts `AESData`, while `dataFromHex` accepts `AESHexData`.

```cpp
using decentralchat::aes::AESData;
using decentralchat::aes::AESHexData;

// Encode a data struct to hex
AESHexData hexData = dataToHex(aesData);

// Encode separated data to hex
AESHexData hexData = dataToHex(aesData.ciphertext, aesData.key, aesData.iv);

// Decode a data struct to raw AES data
AESData aesData = dataFromHex(hexData);

// Decode separated data to raw AES data
AESData aesData = dataFromHex(hexData.ciphertext, hexData.key, hexData.iv);
```
