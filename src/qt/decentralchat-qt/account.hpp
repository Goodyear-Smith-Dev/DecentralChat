#pragma once

#include <string>
#include <QByteArray>
#include <QFile>
#include <QString>

#define LATEST_ACCOUNT_VERSION 1

namespace decentralchat::accounts {

    struct DecryptedData {
        QString password;
        QString privateKey;
    };

    class Account {
    public:

        Account(ushort version, uint64_t id, QString name, QString displayName, QString publicKey, QByteArray encrypted);

        QByteArray toBytes();
        bool decrypt(std::string password, DecryptedData& decrypted);

        ushort getVersion() const { return version; }
        uint64_t getId() const { return id; }
        QString getName() const { return name; }
        QString getDisplayName() const { return displayName; }
        void setDisplayName(QString displayName);
        QString getPublicKey() const { return publicKey; }

    private:
        ushort version;
        uint64_t id;
        QString name;
        QString displayName;
        QString publicKey;
        QByteArray encrypted;

    };

    Account* create(QString name, QString displayName, QString password);
    uint64_t generateId();
    Account* fromBytes(const QByteArray& bytes);
    Account* fromBytes(QFile& file);
}