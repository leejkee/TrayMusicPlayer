//
// Created by cww on 25-2-19.
//
#pragma once
#include <QString>
#include <QFile>
#include <QDebug>


namespace Tray::Ui {
    inline const auto LOCAL_LIST_KEY = QStringLiteral("Local");
}


namespace Tray::Ui::Panel {
    inline const auto PLAY_ALL_KEY = QStringLiteral("Play All");
    inline const auto EXPAND_BTN_TEXT = QStringLiteral("User's");
    constexpr int UNINITIALIZED_VALUE = -1;
}


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
