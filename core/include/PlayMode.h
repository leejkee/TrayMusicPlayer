//
// Created by cww on 25-4-11.
//

#pragma once
#include <QString>

namespace Tray::Core {

        enum class PlayMode {
            Sequential = 0,
            LoopOne    = 1,
            LoopAll    = 2,
            Shuffle    = 3
        };

        static QString PlayModeToString(const PlayMode mode) {
            switch (mode) {
                case PlayMode::Sequential:
                    return QStringLiteral("Sequential");
                case PlayMode::LoopOne:
                    return QStringLiteral("LoopOne");
                case PlayMode::LoopAll:
                    return QStringLiteral("LoopAll");
                case PlayMode::Shuffle:
                    return QStringLiteral("Shuffle");
                default:
                    return {};
            }
        }
}
