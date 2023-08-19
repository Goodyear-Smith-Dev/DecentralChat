#include <chrono>
#include <cstdlib>

#include <QStringDecoder>

#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>

#include "account.hpp"
#include "hashing.hpp"
#include "rsautil.hpp"

//converts x to char*
#define TO_BYTES(x) static_cast<char*>(static_cast<void*>(&x))
//converts char* x to CryptoPP::SecByteBlock given size l
#define TO_SECBYTES(x, l) CryptoPP::SecByteBlock(reinterpret_cast<const CryptoPP::byte*>(x), l)
//converts char* to t*
#define FROM_BYTES(t, x) static_cast<t*>(static_cast<void*>(x))

namespace aes = decentralchat::aes;
namespace accounts = decentralchat::accounts;
namespace hashing = decentralchat::hashing;
namespace rsa = decentralchat::rsa;

accounts::Account::Account(uint16_t version, uint64_t id, const QByteArray& iv, const QString& name, const QString& displayName, const QString& publicKey, const QByteArray& encrypted):
	version(version),
    id(id),
    iv(iv),
    name(name),
    displayName(displayName),
    publicKey(publicKey),
    encrypted(encrypted)
{}

QByteArray accounts::Account::toBytes() {
    QByteArray bytes;
    //2 + 8 + 4 + len(name_bytes) + 4 + len(displayName_bytes) + 4 + len(publicKey_bytes) + 4 + len(encrypted)
    //size = (26 + name_bytes.length() + displayName_bytes.length() + publicKey_bytes.length() + encrypted.length())


    //get bytes and lengths

    QByteArray name_bytes = name.toUtf8();
    uint name_l = name_bytes.length();

    QByteArray displayName_bytes = displayName.toUtf8();
    uint displayName_l = displayName_bytes.length();

    QByteArray publicKey_bytes = publicKey.toUtf8();
    uint publicKey_l = publicKey_bytes.length();

    uint encrypted_l = encrypted.length();

    uint16_t version = LATEST_ACCOUNT_VERSION;
    bytes.append(TO_BYTES(version), 2);
    bytes.append(TO_BYTES(id), 8);
    bytes.append(TO_BYTES(name_l), 4);
    bytes.append(TO_BYTES(displayName_l), 4);
    bytes.append(TO_BYTES(publicKey_l), 4);
    bytes.append(TO_BYTES(encrypted_l), 4);
    bytes.append(name_bytes);
    bytes.append(displayName_bytes);
    bytes.append(publicKey_bytes);
    bytes.append(encrypted);

    return bytes;
}

//TODO for encryption: https://stackoverflow.com/a/45684139 + https://stackoverflow.com/q/5930056 (the question not the answer)

bool accounts::Account::decrypt(const std::string& password, accounts::DecryptedData& decrypted) {
    switch (version) {
        case 1: {
            //hash to bytes, not hex

            CryptoPP::SecByteBlock hashword(CryptoPP::SHA256::DIGESTSIZE);
            CryptoPP::SHA256().CalculateDigest(hashword, reinterpret_cast<const CryptoPP::byte*>(password.c_str()), password.length());

            //key and iv from hex

            QString decryptedData;
            try {
                decryptedData = QString::fromStdString(aes::decrypt(
                    std::string(encrypted.data(), encrypted.size()),
                    hashword,
                    TO_SECBYTES(iv.data(), iv.size())
                ));
            }
            catch(...) {
				return false;
			}

            decrypted = accounts::DecryptedData{ decryptedData.sliced(0, password.length()), decryptedData.sliced(password.length()) };

            //verify
            return decrypted.password.toStdString() == password;
        }
        default:
            return false;
    }
}

void accounts::Account::save(QFile& file, const std::string& password) noexcept(false) {
    if (!(file.isOpen() || file.open(QFile::WriteOnly)))
        return;

    //write bytes size + bytes
    const QByteArray bytes = toBytes();
    uint bytes_l = bytes.size();
    file.write(TO_BYTES(bytes_l), 4);
    file.write(bytes);

    //write hash (always 256)

    CryptoPP::byte hash[CryptoPP::SHA256::DIGESTSIZE];
    CryptoPP::SHA256().CalculateDigest(hash, reinterpret_cast<const CryptoPP::byte*>(bytes.data()), bytes.size());
    const char* charhash = reinterpret_cast<char*>(hash);
    file.write(charhash, CryptoPP::SHA256::DIGESTSIZE);

    //write signature

    DecryptedData decrypted;
    if (!decrypt(password, decrypted))
        return; //TODO fail

    CryptoPP::RSA::PrivateKey privateKey = rsa::keyFromBase64(decrypted.privateKey.toStdString());
    CryptoPP::SecByteBlock signature = rsa::sign(std::string(charhash), privateKey);
    file.write(reinterpret_cast<const char*>(signature.data()), signature.size());

    file.close();
}

void accounts::Account::setDisplayName(QString displayName) {
    this->displayName = displayName;
}

void accounts::Account::setEncrypted(const QByteArray& encrypted) {
    this->encrypted = encrypted;
}

void accounts::Account::setEncrypted(const accounts::DecryptedData& decrypted) noexcept(false) {
    aes::AESData data = accounts::encrypt(decrypted, TO_SECBYTES(iv.data(), iv.size()));
    setEncrypted(QByteArray(data.ciphertext.c_str(), data.ciphertext.size()));
}

accounts::Account* accounts::create(QString name, QString displayName, QString password) {
    //generate rsa keypair and aes iv

    aes::AESData data;
    CryptoPP::RSAFunction pre_publicKey, pre_privateKey;
    CryptoPP::InvertibleRSAFunction params;
    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
    try {
        //NOTE: this lags the ui thread (main thread)
        //std::tie(pre_publicKey, pre_privateKey, params) = rsa::generateKeys();
    }
    catch(...) {
		return nullptr;
	}

    QString publicKey = "";//QString::fromStdString(rsa::keyToBase64(pre_publicKey));
    QString privateKey = "";//QString::fromStdString(rsa::keyToBase64(pre_privateKey));

    //encrypt password and private key

    CryptoPP::AutoSeededRandomPool rng;
    accounts::DecryptedData decrypted{password, privateKey};
    try {
        rng.GenerateBlock(iv, iv.size());
        data = accounts::encrypt(decrypted, iv);
    }
    catch(...) {
		return nullptr;
	}

    return new Account(
        LATEST_ACCOUNT_VERSION,
        generateId(),
        QByteArray(reinterpret_cast<char*>(data.iv.data()), data.iv.size()),
        name,
        displayName,
        publicKey,
        QByteArray(data.ciphertext.c_str(), data.ciphertext.size()));
}

uint64_t getNow() { return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); }

uint64_t accounts::generateId() {
    uint64_t ms = getNow();
    return ms * 1000 + (rand() % 1000);
}

accounts::Account* accounts::fromBytes(const QByteArray& bytes) {
    uint16_t version = *FROM_BYTES(uint16_t, bytes.sliced(0, 2).data()); //len 2
    switch (version) {
        case 1: {
            uint64_t id = *FROM_BYTES(uint64_t, bytes.sliced(2, 8).data()); //len 8
            uint name_l = *FROM_BYTES(uint, bytes.sliced(10, 4).data()); // len 4
            uint displayName_l = *FROM_BYTES(uint, bytes.sliced(14, 4).data());
            uint publicKey_l = *FROM_BYTES(uint, bytes.sliced(18, 4).data());
            uint encrypted_l = *FROM_BYTES(uint, bytes.sliced(22, 4).data());
            QByteArray iv = bytes.sliced(26, 16);

            int index = 42; //keep track of where to start reading each "block"
            QString name = QString::fromUtf8(bytes.sliced(index, name_l));
            QString displayName = QString::fromUtf8(bytes.sliced(index+=name_l, displayName_l));
            QString publicKey = QString::fromUtf8(bytes.sliced(index+=displayName_l, publicKey_l));
            QByteArray encoded = bytes.sliced(index+=publicKey_l, encrypted_l);
            return new Account(version, id, iv, name, displayName, publicKey, encoded);
        }
        default:
            return nullptr;
    }
}

aes::AESData accounts::encrypt(const accounts::DecryptedData& decrypted, const CryptoPP::SecByteBlock& iv) noexcept(false) {
    CryptoPP::SecByteBlock key(CryptoPP::SHA256::DIGESTSIZE);
    CryptoPP::SHA256().CalculateDigest(key, reinterpret_cast<const CryptoPP::byte*>(decrypted.password.data()), decrypted.password.length());

    CryptoPP::CBC_Mode<AES>::Encryption encrypter;
	encrypter.SetKeyWithIV(key, key.size(), iv);

    std::string ciphertext;
    std::string password = decrypted.password.toStdString();
    std::string privateKey = decrypted.privateKey.toStdString();
    std::string plaintext = password + privateKey;

	CryptoPP::StringSource ssource(plaintext, true,
		new CryptoPP::StreamTransformationFilter(encrypter,
			new CryptoPP::StringSink(ciphertext)
		)
	);

	auto data = aes::AESData{ciphertext, key, iv};
    return data;
}

accounts::Account* accounts::load(const QByteArray& bytes) {
    uint accountBytes_l = *FROM_BYTES(uint, bytes.sliced(0, 4).data());
    QByteArray accountBytes = bytes.sliced(4, accountBytes_l);

    //check hash
    CryptoPP::SecByteBlock accountHash(CryptoPP::SHA256::DIGESTSIZE);
    CryptoPP::SHA256().CalculateDigest(accountHash, reinterpret_cast<const CryptoPP::byte*>(accountBytes.data()), accountBytes.size());
    QByteArray storedHash_pre = bytes.sliced(accountBytes_l, CryptoPP::SHA256::DIGESTSIZE);
    CryptoPP::SecByteBlock storedHash = TO_SECBYTES(storedHash_pre.data(), storedHash_pre.size());
    if (storedHash!=accountHash)
        return nullptr;

    //get account
    Account* acct = accounts::fromBytes(accountBytes);
    if (acct==nullptr)
        return nullptr;

    //verify signature
    QByteArray signature_pre = bytes.sliced(accountBytes_l + CryptoPP::SHA256::DIGESTSIZE);
    CryptoPP::SecByteBlock signature = TO_SECBYTES(signature_pre.data(), signature_pre.size());
    CryptoPP::RSAFunction publicKey = rsa::keyFromBase64(acct->getPublicKey().toStdString());
    if (!rsa::verify(std::string(storedHash_pre.data(), storedHash_pre.size()), signature, publicKey)) {
        delete acct; //discard invalid account
        return nullptr;
    }

    return acct;
}

accounts::Account* accounts::load(QFile& file) {
    if (!(file.isOpen() || file.open(QFile::ReadOnly)))
        return nullptr;
    return accounts::load(file.readAll());
}