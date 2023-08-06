#pragma once

#include <QFile>
#include <QString>
#include <unordered_map>

namespace decentralchat::styles {
    using stylevars = std::unordered_map<QString, QString>;

    void loadStyleVars(QFile& file, stylevars& vars);
    QString applyStyle(const QString& styleSheet, const stylevars& vars);

};