//
// Created by cww on 25-4-1.
//
#pragma once
#include <QStringList>


namespace Core {
    inline const QStringList MUSIC_FILTERS = {
        QStringLiteral("*.mp3"),
        QStringLiteral("*.flac")
    };

    inline const QStringList IMAGE_FILTERS = {
        QStringLiteral("*.png"), QStringLiteral("*.jpeg")
    };

    inline constexpr int UNINITIALIZED_VALUE = -1;

    // inline const QString SETTINGS_WIN32 = QStringLiteral("C:/Users/cww/Documents/Workspace/TrayMusicPlayer/core/settings/init_win32.json");
    inline const auto SETTINGS_WIN32 =  QStringLiteral("C:/Users/cww/Documents/Workspace/TrayMusicPlayer/core/settings/init_win32.json");
    inline const auto SETTINGS_LINUX = QStringLiteral(":/core/settings/init_linux.json");
    inline const auto DB_PATH = QStringLiteral("C:/Users/cww/Documents/Workspace/TrayMusicPlayer/core/music.db");
    inline const auto LOCAL_LIST_KEY = QStringLiteral("Local");
}
