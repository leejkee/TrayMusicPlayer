//
// Created by cww on 25-2-19.
//
#pragma once
#include <QString>
#include <QFile>
#include <QDebug>

namespace Tray::Ui {
    inline QString readQSS(const QString &qssPath) {
        if (QFile file(qssPath); file.open(QFile::ReadOnly)) {
            QString qss = QString::fromUtf8(file.readAll()).trimmed();
            file.close();
            return qss;
        }
        qWarning() << "Failed to load QSS file:" << qssPath;
        return {};
    }
}
