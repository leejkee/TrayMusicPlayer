//
// Created by cww on 25-4-6.
//
#include <playlist/playlist.h>
#include <log/log.h>
#include <QRandomGenerator>

namespace Tray::Core
{

Playlist::Playlist(QObject* parent)
    : QObject(parent)
{
    m_index = 0;
    m_playMode = PlayMode::Sequential;
    setObjectName(QStringLiteral("PlayList"));
    LOG_INFO("Playlist initialized with empty, Sequential mode");
}

Playlist::~Playlist() = default;

void Playlist::loadPlaylist(const QString& listKey
                             , const std::shared_ptr<QList<MusicMetaData> > playlist)
{
    m_currentListKey = listKey;
    m_musicList = playlist;
    Q_EMIT signalNotifyUiCurrentPlaylistKeyChanged(listKey);
    LOG_INFO(QString("Load playlist [%1] successfully").arg(listKey));
}

qsizetype Playlist::nextMusic()
{
    setCurrentMusicIndex(getNextMusicIndex());
    return m_index;
}

qsizetype Playlist::preMusic()
{
    setCurrentMusicIndex(getPreMusicIndex());
    return m_index;
}

qsizetype Playlist::getNextMusicIndex() const
{
    switch (m_playMode)
    {
    case PlayMode::Sequential:
    case PlayMode::LoopAll:
        return (m_index + 1) % m_musicList->size();
    case PlayMode::LoopOne:
        return m_index;
    case PlayMode::Shuffle:
        return QRandomGenerator::global()->bounded(m_musicList->size());
    default:
        return 0;
    }
}

qsizetype Playlist::getPreMusicIndex() const
{
    switch (m_playMode)
    {
    case PlayMode::Sequential:
    case PlayMode::LoopAll:
        return (m_index == 0)
               ? m_musicList->size() - 1
               : m_index - 1;
    case PlayMode::LoopOne:
        return m_index;
    case PlayMode::Shuffle:
        return QRandomGenerator::global()->bounded(m_musicList->size());
    default:
        return 0;
    }
}


qsizetype Playlist::getCurrentMusicIndex() const
{
    return m_index;
}

void Playlist::setCurrentMusicIndex(const qsizetype index)
{
    if (index >= m_musicList->size())
    {
        LOG_ERROR("Index out of range");
    }
    m_index = index;
    LOG_INFO(QString("index changed: %1").arg(QString::number(m_index)));
}

void Playlist::setPlayMode(const PlayMode playMode)
{
    if (m_playMode != playMode)
    {
        m_playMode = playMode;
        Q_EMIT signalPlayModeChanged(static_cast<int>(playMode));
        LOG_INFO(QString("Play mode changed: %1").arg(PlayModeToString(playMode)));
    }
}

void Playlist::changePlayMode()
{
    switch (m_playMode)
    {
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

QString Playlist::getListKey() const
{
    return m_currentListKey;
}

MusicMetaData Playlist::currentMusic() const
{
    return m_musicList->at(m_index);
}

MusicMetaData Playlist::getMusic(const qsizetype index) const
{
    return m_musicList->at(index);
}

}
