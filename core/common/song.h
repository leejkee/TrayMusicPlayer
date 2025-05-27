#pragma once
#include <QString>

namespace Tray::Core {

struct Song {
    Song() = default;
    Song(const Song& song) = default;
    explicit Song(const QString& songFilePath);
    static QString removeSuffix(const QString& str);

    QString m_name;
    QString m_artist;
    QString m_title;
    QString m_path;
    int m_duration{0};

    ///
    /// @param path should be the @m_path
    /// @return the length of this music(Format: 00:00)
    static int musicLength(const QString& path);

    static QString convertToArtist(const QString& str);

    static QString convertToName(const QString& str);

    void songInitByTagLib(const QString& path);
};
} // namespace Tray::Core
