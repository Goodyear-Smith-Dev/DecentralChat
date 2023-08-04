#pragma once

#include <QFile>
#include <QString>
#include <unordered_map>


namespace decentralchat::styles {
    typedef std::unordered_map<QString, QString> stylevars;

    void loadStyleVars(QFile& file, stylevars& vars);
    QString applyStyle(QString styleSheet, stylevars& vars);

};