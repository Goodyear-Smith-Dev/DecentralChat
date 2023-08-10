#include "mainwindow.hpp"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QString>
#include <QStringList>

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

    return app.exec();
}
