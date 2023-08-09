#include "mainwindow.hpp"
#include "rsautil.hpp"
#include "aesutil.hpp"
#include "hashing.hpp"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QString>
#include <QStringList>

namespace aes = decentralchat::aes;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString& locale: uiLanguages) {
        const QString baseName = "DecentralChat_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    MainWindow window;
    window.show();

	// auto&& [keys, publicKey, privateKey] = rsa::generateKeys();
	// rsa::saveKey(publicKey, "pubkey.key");

	// CryptoPP::ByteQueue queue;
	// publicKey.Save(queue);
	// rsautil::saveBase64("pubkey.key", queue);

	// queue.Clear();

	// privateKey.Save(queue);
	// rsautil::saveBase64("privkey.key", queue);

	std::string message = "Hello, World!";
	// auto signature = rsautil::sign(message, privateKey);
	// bool verified = rsautil::verify(message, signature, publicKey);
	// std::cout << verified << "\n";

	// auto ciphertext = rsautil::encrypt(message, publicKey);
	// auto recovered = rsautil::decrypt(ciphertext, privateKey);
	// std::cout << recovered << "\n";

	auto&& [ciphertext, key, iv] = aes::encrypt(message);
	std::cout << key.BytePtr() << "\n";
	auto recovered = aes::decrypt(ciphertext, key, iv);
	std::cout << recovered << "\n";

	auto hexData = aes::dataToHex(ciphertext, key, iv);
	std::cout << hexData.ciphertext << "\n" << hexData.key << "\n" << hexData.iv << "\n";

	auto data = aes::dataFromHex(hexData);
	std::cout << data.ciphertext << "\n" << data.key.BytePtr() << "\n" << data.iv.BytePtr() << "\n";

	std::cout << decentralchat::hashing::SHA256("abc") << "\n";

    return 0;//app.exec();
}
