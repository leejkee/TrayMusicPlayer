#include "taglib/fileref.h"
#include <Song.h>

namespace Core::Service{

Song::Song(const QString& songFilePath)
{
    if (!songFilePath.isEmpty()) {
        m_path = songFilePath;
        m_fullName = removeSuffix(m_path);
        m_duration = musicLength(m_path);
    } else {
        m_fullName = "";
        m_path = "";
        m_duration = 0;
    }
    m_logoIndex = 0;
}

QString Song::removeSuffix(const QString& str)
{
    const auto s = str.right(str.size() - str.lastIndexOf("/") - 1);
    return s.left(s.indexOf("."));
}

int Song::musicLength(const QString& path)
{
#if defined(_WIN32)
    const std::wstring tg_path = path.toStdWString();
#elif defined(__linux__)
    const std::string tg_path = path.toStdString();
#endif
    if (const TagLib::FileRef f(tg_path.c_str()); !f.isNull() && f.audioProperties()) {
        const TagLib::AudioProperties* properties = f.audioProperties();
        return properties->lengthInSeconds();
    }
    return {};
}

}