#pragma once

#include <string>

#include <QByteArray>
#include <QFile>
#include <QString>

#include "aesutil.hpp"

namespace decentralchat::accounts {
	constexpr uint16_t LATEST_ACCOUNT_VERSION = 1;
	constexpr const char* LOCAL_ACCT_FILE = "local.acct";

    // Decrypted private account data
    struct DecryptedData {
        QString password;
        QString privateKey;
    };

    class Account {
    public:
        Account(uint16_t version, uint64_t id, const QByteArray& iv, const QString& name, const QString& displayName, const QString& publicKey, const QByteArray& encrypted);

        //Convert the account's state to bytes
        QByteArray toBytes();
        //Decrypt the account's encrypted data
        bool decrypt(const std::string& password, DecryptedData& decrypted);
        //Write the account bytes (unhashed) + hashed + signature
        void save(QFile& file, const std::string& password) noexcept(false);

        uint16_t getVersion() const { return version; }
        uint64_t getId() const { return id; }
        const QByteArray& getIv() const { return iv; }
        const QString& getName() const { return name; }
        const QString& getDisplayName() const { return displayName; }
        void setDisplayName(QString displayName);
        const QString& getPublicKey() const { return publicKey; }
        const QByteArray& getEncrypted() const { return encrypted; }
        void setEncrypted(const QByteArray& encrypted);
        void setEncrypted(const DecryptedData& decrypted) noexcept(false);

    private:
        uint16_t version;
        uint64_t id;
        QByteArray iv;
        QString name;
        QString displayName;
        QString publicKey;
        QByteArray encrypted;

    };

    //Create a new Account with the unspecified values automatically generated
    Account* create(QString name, QString displayName, QString password);
    //Generate the ID for an account
    uint64_t generateId();
    //Reconstruct an account from its byte repesentation
    Account* fromBytes(const QByteArray& bytes);
    //Encrypt decrypted account data
    decentralchat::aes::AESData encrypt(const DecryptedData& data, const CryptoPP::SecByteBlock& iv) noexcept(false);
    //Read the account bytes (unhashed) + hashed + signature from the given QByteArray
    Account* load(const QByteArray& bytes);
    //Read the account bytes (unhashed) + hashed + signature from the given QFile
    Account* load(QFile& file);
}