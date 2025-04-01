//
// Created by cww on 25-4-1.
//
#pragma once

#include <QStringList>

namespace Core {
    inline auto musicFilters() -> const QStringList& {
        static const QStringList filters = {
            QStringLiteral("*.mp3"),
            QStringLiteral("*.flac")
        };
        return filters;
    }

    inline const QStringList IMAGE_FILTERS = {
        QStringLiteral("*.png"), QStringLiteral("*.jpeg")
    };

    inline constexpr int UNINITIALIZED_VALUE = -1;
}