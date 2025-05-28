//
// Created by cww on 25-4-1.
//
#pragma once
#include <QStringList>

namespace Tray::Core {

inline const QStringList IMAGE_FILTERS = {QStringLiteral("*.png"),
                                          QStringLiteral("*.jpeg")};
inline constexpr int UNINITIALIZED_VALUE = -1;

#if defined(Q_OS_WIN32)
inline const auto PROJECT_PATH =
    QStringLiteral("C:/Users/cww/Documents/Workspace/TrayMusicPlayer/");
#elif defined(Q_OS_LINUX)
inline const auto PROJECT_PATH =
    QStringLiteral("/home/aude/workspace/qt/TrayMusicPlayer/");
#endif

inline const auto LOCAL_LIST_KEY = QStringLiteral("Local");
} // namespace Tray::Core
