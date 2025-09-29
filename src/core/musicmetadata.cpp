#include "musicmetadata.h"

namespace Tray::Core
{
MusicMetaData::MusicMetaData(const QString& path)
    : m_path(path),
      m_title(removeSuffix(path))
{
    m_name = convertToName(m_title);
    m_artist = convertToArtist(m_title);
}

MusicMetaData::MusicMetaData(const MusicMetaData& other)
{
    m_path = other.m_path;
    m_title = other.m_title;
    m_artist = other.m_artist;
    m_name = other.m_name;
}

MusicMetaData::MusicMetaData(MusicMetaData&& other) noexcept
{
    m_path = std::move(other.m_path);
    m_title = std::move(other.m_title);
    m_artist = std::move(other.m_artist);
    m_name = std::move(other.m_name);
}

bool MusicMetaData::operator==(const MusicMetaData& other) const
{
    return m_path == other.m_path;
}

bool MusicMetaData::operator!=(const MusicMetaData& other) const
{
    return !(*this == other);
}

MusicMetaData& MusicMetaData::operator=(const MusicMetaData& other)
{
    if (this != &other)
    {
        m_path = other.m_path;
        m_title = other.m_title;
        m_artist = other.m_artist;
        m_name = other.m_name;;
    }
    return *this;
}

MusicMetaData& MusicMetaData::operator=(MusicMetaData&& other) noexcept
{
    if (this != &other)
    {
        m_path = std::move(other.m_path);
        m_title = std::move(other.m_title);
        m_artist = std::move(other.m_artist);
        m_name = std::move(other.m_name);
    }
    return *this;
}

QString MusicMetaData::removeSuffix(const QString& str)
{
    const auto s = str.right(str.size() - str.lastIndexOf("/") - 1);
    return s.left(s.indexOf("."));
}

QString MusicMetaData::convertToArtist(const QString& str)
{
    return str.right(str.length() - str.indexOf("-") - 1).trimmed();
}

QString MusicMetaData::convertToName(const QString& str)
{
    return str.left(str.indexOf("-")).trimmed();
}
}
