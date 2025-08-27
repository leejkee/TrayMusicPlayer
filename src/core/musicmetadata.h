#pragma once
#include <QString>
#include <QMetaType>

namespace Tray::Core
{

struct  MusicMetaData
{
    MusicMetaData() = default;

    explicit MusicMetaData(const QString& path);

    ~MusicMetaData() = default;

    MusicMetaData(const MusicMetaData& other);

    MusicMetaData(MusicMetaData&& other) noexcept;

    bool operator==(const MusicMetaData& other) const;

    bool operator!=(const MusicMetaData& other) const;

    MusicMetaData& operator=(const MusicMetaData& other);

    MusicMetaData& operator=(MusicMetaData&& other) noexcept;

    /// @param path should be the @m_path
    /// @return the length of this music(Format: 00:00)
    static int musicLength(const QString& path);

    static QString convertToArtist(const QString& str);

    static QString convertToName(const QString& str);

    static QString removeSuffix(const QString& str);

    void songInitByTagLib(const QString& path);

    QString m_path;
    QString m_title;
    QString m_name;
    QString m_artist;
    int m_duration;
};

} // namespace Tray::Core
Q_DECLARE_METATYPE(Tray::Core::MusicMetaData);
