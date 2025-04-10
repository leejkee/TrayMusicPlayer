//
// Created by cww on 25-4-6.
//
#include <PlayList.h>
#include <CoreConstants.h>

namespace Core::Service {

    PlayList::PlayList(const QVector<Song> &musicList, QObject *parent)
        : QObject(parent)
        , m_index(UNINITIALIZED_VALUE)
        , m_musicList(musicList) {
        setObjectName(QStringLiteral("PlayList"));
        Log = Logger_QT(this->objectName());
    }

    void PlayList::loadMusicList(const QVector<Song> &musicList) {
        if (musicList.isEmpty()) {
            Log.log(Logger_QT::LogLevel::Warning, "load empty musicList");
        }
        m_musicList.clear();
        m_musicList = musicList;
    }

    QString PlayList::getCurrentMusicPath() const {
        return m_musicList.at(m_index).getPath();
    }

    QString PlayList::getCurrentMusicTitle() const {
        return m_musicList.at(m_index).getTitle();
    }

    int PlayList::getCurrentMusicIndex() const {
        return m_index;
    }

    void PlayList::setCurrentMusicIndex(const int index) {
        if (index >= m_musicList.size()) {
            Log.log(Logger_QT::LogLevel::Error, "index out of range");
        }
        if (m_index != index) {
            m_index = index;
            Q_EMIT signalMusicNameChanged(m_musicList.at(m_index).getTitle());
            Q_EMIT signalMusicDurationChanged(m_musicList.at(m_index).getDuration());
            Q_EMIT signalMusicIndexChanged(m_index);
        }
    }

}
