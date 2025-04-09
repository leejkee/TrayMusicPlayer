#include "taglib/fileref.h"
#include <Song.h>


namespace Core::Service {
    Song::Song(const QString &songFilePath) {
        m_path = songFilePath;
        m_title = removeSuffix(m_path);
        m_Name = convertToName(m_title);
        m_Artist = convertToArtist(m_title);
        m_duration = musicLength(m_path);

        // todo
        // songInitByTagLib(m_path);
        // if (m_title.isEmpty()) {
        // }
    }

    QString Song::removeSuffix(const QString &str) {
        const auto s = str.right(str.size() - str.lastIndexOf("/") - 1);
        return s.left(s.indexOf("."));
    }

    void Song::songInitByTagLib(const QString &path) {
#if defined(_WIN32)
        const std::wstring tg_path = path.toStdWString();
#elif defined(__linux__)
        const std::string tg_path = path.toStdString();
#endif
        if (const TagLib::FileRef f(tg_path.c_str()); !f.isNull() && f.audioProperties()) {
            const TagLib::AudioProperties *properties = f.audioProperties();
            const TagLib::Tag *tag = f.tag();
            m_duration = properties->lengthInSeconds();
            m_Name = QString::fromStdString(tag->title().to8Bit());
            m_Artist = QString::fromStdString(tag->artist().to8Bit());
            m_title = QString::fromStdString(tag->title().to8Bit());
        }
    }


    int Song::musicLength(const QString &path) {
#if defined(_WIN32)
        const std::wstring tg_path = path.toStdWString();
#elif defined(__linux__)
    const std::string tg_path = path.toStdString();
#endif
        if (const TagLib::FileRef f(tg_path.c_str()); !f.isNull() && f.audioProperties()) {
            const TagLib::AudioProperties *properties = f.audioProperties();
            return properties->lengthInSeconds();
        }
        return {};
    }

    QString Song::convertToArtist(const QString &str) {
        return str.right(str.length() - str.indexOf("-") - 1).trimmed();
    }

    QString Song::convertToName(const QString &str) {
        return str.left(str.indexOf("-")).trimmed();
    }
}
