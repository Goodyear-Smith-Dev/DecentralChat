#include "account.hpp"

#include <chrono>
#include <cstdlib>
#include <QStringDecoder>

//converts x to char*
#define TO_BYTES(x) static_cast<char*>(static_cast<void*>(&x))
//converts char* to t*
#define FROM_BYTES(t, x) static_cast<t*>(static_cast<void*>(x))

namespace accounts = decentralchat::accounts;

uint64_t getNow() { return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); }


accounts::Account* accounts::Account::create(QString name, QString displayName, QString password) {
    //TODO generate rsa keypair
    QString publicKey;
    QString privateKey;
    //TODO encrypt password and private key
    QByteArray encrypted;
    return new Account(LATEST_ACCOUNT_VERSION, generateId(), name, displayName, publicKey, encrypted);
}

accounts::Account::Account(ushort version, uint64_t id, QString name, QString displayName, QString publicKey, QByteArray encrypted) {
    this->version = version;
    this->id = id;
    this->name = name;
    this->displayName = displayName;
    this->publicKey = publicKey;
    this->encrypted = encrypted;
}

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

    bytes.append(TO_BYTES(version), 2);
    bytes.append(TO_BYTES(id), 8);
    bytes.append(TO_BYTES(name_l), 4);
    bytes.append(name_bytes);
    bytes.append(TO_BYTES(displayName_l), 4);
    bytes.append(displayName_bytes);
    bytes.append(TO_BYTES(publicKey_l), 4);
    bytes.append(publicKey_bytes);
    bytes.append(TO_BYTES(encrypted_l), 4);
    bytes.append(encrypted);

    return bytes;
}

//TODO add later
bool accounts::Account::decrypt(std::string password, accounts::DecryptedData& decrypted) {
    switch (version) {
        default:
            return false;
    }
}

uint64_t accounts::generateId() {
    uint64_t ms = getNow();
    return ms * 1000 + (rand() % 1000);
}

accounts::Account* accounts::fromBytes(const QByteArray& bytes) {
    ushort version = *FROM_BYTES(ushort, bytes.sliced(0, 2).data()); //len 2
    switch (version) {
        case 1: {
            uint64_t id = *FROM_BYTES(uint64_t, bytes.sliced(2, 8).data()); //len 8
            uint name_l = *FROM_BYTES(uint, bytes.sliced(10, 4).data()); // len 4
            int index = 14; //from here on, where each block starts depends on the length of the previous block
            QString name = QString::fromUtf8(bytes.sliced(index, name_l));
            uint displayName_l = *FROM_BYTES(uint, bytes.sliced(index+=name_l, 4).data());
            QString displayName = QString::fromUtf8(bytes.sliced(index+=4, displayName_l));
            uint publicKey_l = *FROM_BYTES(uint, bytes.sliced(index+=displayName_l, 4).data());
            QString publicKey = QString::fromUtf8(bytes.sliced(index+=4, publicKey_l));
            uint encrypted_l = *FROM_BYTES(uint, bytes.sliced(index+=publicKey_l, 4).data());
            return new Account(version, id, name, displayName, publicKey, bytes.sliced(index+4, encrypted_l));
        }
        default:
            return nullptr;
    }
}

accounts::Account* accounts::fromBytes(QFile& file) {
    if (!(file.isOpen() || file.open(QFile::ReadOnly)))
        return nullptr;
    QByteArray bytes = file.readAll();
    file.close();
    return accounts::fromBytes(bytes);
}