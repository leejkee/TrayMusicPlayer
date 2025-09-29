#pragma once
#include <QString>
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

    static QString convertToArtist(const QString& str);

    static QString convertToName(const QString& str);

    static QString removeSuffix(const QString& str);


    QString m_path;
    QString m_title;
    QString m_name;
    QString m_artist;
};

} // namespace Tray::Core
// Q_DECLARE_METATYPE(Tray::Core::MusicMetaData)
