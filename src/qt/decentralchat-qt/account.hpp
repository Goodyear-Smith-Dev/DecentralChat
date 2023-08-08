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
        static Account* create(QString name, QString displayName, QString password);

        Account(ushort version, uint64_t id, QString name, QString displayName, QString publicKey, QByteArray encrypted);

        QByteArray toBytes();
        bool decrypt(std::string password, DecryptedData& decrypted);

    private:
        ushort version;
        uint64_t id;
        QString name;
        QString displayName;
        QString publicKey;
        QByteArray encrypted;

    };

    uint64_t generateId();
    Account* fromBytes(const QByteArray& bytes);
    Account* fromBytes(QFile& file);
}