#include "musicmetadata.h"
#include <taglib/fileref.h>

namespace Tray::Core
{
MusicMetaData::MusicMetaData(const QString& path)
    : m_path(path),
      m_title(removeSuffix(path)),
      m_name(convertToName(path)),
      m_artist(convertToArtist(path)),
      m_duration(musicLength(path))
{
}

MusicMetaData::MusicMetaData(const MusicMetaData& other)
{
    m_path = other.m_path;
    m_title = other.m_title;
    m_artist = other.m_artist;
    m_duration = other.m_duration;
    m_name = other.m_name;
}

MusicMetaData::MusicMetaData(MusicMetaData&& other) noexcept
{
    m_path = std::move(other.m_path);
    m_title = std::move(other.m_title);
    m_artist = std::move(other.m_artist);
    m_duration = std::move(other.m_duration);
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
        m_duration = other.m_duration;
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
        m_duration = std::move(other.m_duration);
        m_name = std::move(other.m_name);
    }
    return *this;
}

QString MusicMetaData::removeSuffix(const QString& str)
{
    const auto s = str.right(str.size() - str.lastIndexOf("/") - 1);
    return s.left(s.indexOf("."));
}

void MusicMetaData::songInitByTagLib(const QString& path)
{
#if defined(_WIN32)
    const std::wstring tg_path = path.toStdWString();
#elif defined(__linux__)
        const std::string tg_path = path.toStdString();
#endif
    if (const TagLib::FileRef f(tg_path.c_str()); !f.isNull() && f.
        audioProperties())
    {
        const TagLib::AudioProperties* properties = f.audioProperties();
        const TagLib::Tag* tag = f.tag();
        m_duration = properties->lengthInSeconds();
        m_name = QString::fromStdString(tag->title().to8Bit());
        m_artist = QString::fromStdString(tag->artist().to8Bit());
        m_title = QString::fromStdString(tag->title().to8Bit());
    }
}


int MusicMetaData::musicLength(const QString& path)
{
#if defined(_WIN32)
    const std::wstring tg_path = path.toStdWString();
#elif defined(__linux__)
    const std::string tg_path = path.toStdString();
#endif
    if (const TagLib::FileRef f(tg_path.c_str()); !f.isNull() && f.
        audioProperties())
    {
        const TagLib::AudioProperties* properties = f.audioProperties();
        return properties->lengthInSeconds();
    }
    return {};
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
