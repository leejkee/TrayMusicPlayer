//
// Created by cww on 25-4-1.
//
#pragma once
#include <QStringList>

namespace Tray::Core {

    inline const QStringList IMAGE_FILTERS = {
        QStringLiteral("*.png"), QStringLiteral("*.jpeg")
    };

    inline constexpr int UNINITIALIZED_VALUE = -1;
    inline const auto SETTINGS_WIN32_PATH =  QStringLiteral("data/settings/init_win32.json");
    inline const auto SETTINGS_LINUX = QStringLiteral("data/settings/init_linux.json");

    inline const auto DB_PATH = QStringLiteral("C:/Users/cww/Documents/Workspace/TrayMusicPlayer/core/music.db");
    inline const auto LOCAL_LIST_KEY = QStringLiteral("Local");
}
