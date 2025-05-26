//
// Created by cww on 25-4-6.
//
#include "playlist.h"
#include <trayconfig.h>
#include <QRandomGenerator>


namespace Tray::Core {
    PlayList::PlayList(QObject *parent)
        : QObject(parent)
          , m_index(UNINITIALIZED_VALUE)
          , m_playMode(PlayMode::Sequential) {
        m_currentListKey = {};
        m_musicList = {};
        setObjectName(QStringLiteral("PlayList"));
        Log = Log::QLogger(this->objectName());
        Log.log(Log::QLogger::LogLevel::Info, "PlayList initialized with empty list, Sequential mode in constructor");
    }

    void PlayList::loadMusicList(const QString &listKey, const QList<Song> &musicList) {
        if (musicList.isEmpty()) {
            Log.log(Log::QLogger::LogLevel::Warning, "load empty musicList");
        }
        m_currentListKey = listKey;
        m_musicList = musicList;
        Q_EMIT signalNotifyUiCurrentPlaylistKeyChanged(listKey);
        Log.log(Log::QLogger::LogLevel::Info, "load musicList successfully");
    }

    void PlayList::nextMusic() {
        setCurrentMusicIndex(getNextMusicIndex());
    }

    void PlayList::preMusic() {
        setCurrentMusicIndex(getPreMusicIndex());
    }

    qsizetype PlayList::getNextMusicIndex() const {
        switch (m_playMode) {
            case PlayMode::Sequential:
            case PlayMode::LoopAll:
                return (m_index + 1) % m_musicList.size();
            case PlayMode::LoopOne:
                return m_index;
            case PlayMode::Shuffle:
                return QRandomGenerator::global()->bounded(m_musicList.size());
            default:
                return 0;
        }
    }

    qsizetype PlayList::getPreMusicIndex() const {
        switch (m_playMode) {
            case PlayMode::Sequential:
            case PlayMode::LoopAll:
                return (m_index == 0) ? m_musicList.size() - 1 : m_index - 1;
            case PlayMode::LoopOne:
                return m_index;
            case PlayMode::Shuffle:
                return QRandomGenerator::global()->bounded(m_musicList.size());
            default:
                return 0;
        }
    }

    QString PlayList::getCurrentMusicPath() const {
        return m_musicList.at(m_index).m_path;
    }

    qsizetype PlayList::getCurrentMusicIndex() const {
        return m_index;
    }

    void PlayList::setCurrentMusicIndex(const qsizetype index) {
        if (index >= m_musicList.size()) {
            Log.log(Log::QLogger::LogLevel::Error, "index out of range");
        }
        m_index = index;
        Log.log(Log::QLogger::LogLevel::Info, "index changed: " + QString::number(m_index));
        Q_EMIT signalCurrentMusicChanged(m_index, m_musicList.at(m_index).m_title, m_musicList.at(m_index).m_duration);
    }

    void PlayList::setPlayMode(const PlayMode playMode) {
        if (m_playMode != playMode) {
            m_playMode = playMode;
            Q_EMIT signalPlayModeChanged(static_cast<int>(playMode));
            Log.log(Log::QLogger::LogLevel::Info, "play mode changed: " + PlayModeToString(playMode));
        }
    }

    void PlayList::changePlayMode() {
        switch (m_playMode) {
            case PlayMode::Sequential:
                setPlayMode(PlayMode::LoopOne);
                break;
            case PlayMode::LoopOne:
                setPlayMode(PlayMode::LoopAll);
                break;
            case PlayMode::LoopAll:
                setPlayMode(PlayMode::Shuffle);
                break;
            case PlayMode::Shuffle:
                setPlayMode(PlayMode::Sequential);
                break;
            default:
                break;
        }
    }

    QString PlayList::getListKey() const {
        return m_currentListKey;
    }

}
