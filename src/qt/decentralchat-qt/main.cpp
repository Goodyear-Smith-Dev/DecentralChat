#include "mainwindow.hpp"

#include <QApplication>
#include <QFile>
#include <QLatin1String>
#include <QList>
#include <QLocale>
#include <QTranslator>
#include <QStackedWidget>
#include <QString>
#include <QStringList>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //load style
    QFile file(":/i18n/styles/decentralchat.light.qss");
    file.open(QFile::ReadOnly);
    QString stylesheet = QLatin1String(file.readAll());
    file.close();

    //set style
    app.setStyleSheet(stylesheet);

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
    return app.exec();
}
