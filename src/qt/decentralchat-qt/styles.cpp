#include "styles.hpp"

#include <QFile>
#include <QLatin1String>
#include <QString>
#include <QStringList>
#include <QTextStream>

using namespace decentralchat;

void styles::loadStyleVars(QFile& file, styles::stylevars& vars) {
    if (!file.isOpen()) {
        file.open(QFile::ReadOnly);
    }

    QTextStream in(&file);
    for (QString line = in.readLine(); !line.isNull(); line=in.readLine()) {
        line = line.trimmed();
        if (line.startsWith("#")) { continue; } //comment
        if (line.startsWith("=")) { continue; } //TODO invalid syntax

        QStringList list = line.split("=");
        QString name = list.takeFirst().trimmed();
        QString value = list.join("=").trimmed();
        for (auto pair : vars) {
            value = value.replace(pair.first, pair.second);   
        }
        vars[name] = value;
    }
    file.close();
}

QString styles::applyStyle(QString styleSheet, styles::stylevars& vars) {
    QString rtv = styleSheet;
    for (auto pair : vars) {
        rtv.replace(pair.first, pair.second);
    }
    return rtv;
}