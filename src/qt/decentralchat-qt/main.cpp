#include "mainwindow.hpp"
#include "styles.hpp"

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

    //load qss sheet

    QFile sheetFile(":/styles/decentralchat.qss");
    sheetFile.open(QFile::ReadOnly);
    QString stylesheet = QLatin1String(sheetFile.readAll());
    sheetFile.close();

    //load stylevars

    QFile styleFile(":/styles/decentralchat.dark.style");
    decentralchat::styles::stylevars vars;
    decentralchat::styles::loadStyleVars(styleFile, vars); //opens and closes file

    //apply stylevars to stylesheet

    stylesheet = decentralchat::styles::applyStyle(stylesheet, vars);

    //set stylesheet

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
