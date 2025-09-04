//
// Created by cww on 25-4-6.
//
#include <playlist/playlist.h>
#include <log/log.h>
#include <trayconfig.h>
#include <QRandomGenerator>

namespace Tray::Core
{

Playlist::Playlist(QObject* parent)
    : QObject(parent),
      m_index(UNINITIALIZED_VALUE),
      m_playMode(PlayMode::Sequential),
      m_musicList(QList<MusicMetaData>{}),
      m_currentListKey(QString{})
{
    setObjectName(QStringLiteral("PlayList"));
    LOG_INFO("Playlist initialized with empty, Sequential mode");
}

void Playlist::loadPlaylist(const QString& listKey
                             , const QList<MusicMetaData>& playlist)
{
    if (playlist.isEmpty())
    {
        LOG_WARNING("Load empty playlist");
    }
    m_currentListKey = listKey;
    m_musicList = playlist;
    Q_EMIT signalNotifyUiCurrentPlaylistKeyChanged(listKey);
    LOG_INFO(QString("Load playlist %1 successfully").arg(listKey));
}

void Playlist::nextMusic()
{
    setCurrentMusicIndex(getNextMusicIndex());
}

void Playlist::preMusic()
{
    setCurrentMusicIndex(getPreMusicIndex());
}

qsizetype Playlist::getNextMusicIndex() const
{
    switch (m_playMode)
    {
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

qsizetype Playlist::getPreMusicIndex() const
{
    switch (m_playMode)
    {
    case PlayMode::Sequential:
    case PlayMode::LoopAll:
        return (m_index == 0)
               ? m_musicList.size() - 1
               : m_index - 1;
    case PlayMode::LoopOne:
        return m_index;
    case PlayMode::Shuffle:
        return QRandomGenerator::global()->bounded(m_musicList.size());
    default:
        return 0;
    }
}

QString Playlist::getCurrentMusicPath() const
{
    return m_musicList.at(m_index).m_path;
}

qsizetype Playlist::getCurrentMusicIndex() const
{
    return m_index;
}

void Playlist::setCurrentMusicIndex(const qsizetype index)
{
    if (index >= m_musicList.size())
    {
        LOG_ERROR("index out of range");
    }
    m_index = index;
    LOG_INFO(QString("index changed: %1").arg(QString::number(m_index)));
    Q_EMIT signalCurrentMusicChanged(m_index
                                     , m_musicList.at(m_index).m_title
                                     , m_musicList.at(m_index).m_duration);
}

void Playlist::setPlayMode(const PlayMode playMode)
{
    if (m_playMode != playMode)
    {
        m_playMode = playMode;
        Q_EMIT signalPlayModeChanged(static_cast<int>(playMode));
        LOG_INFO(QString("play mode changed: %1").arg(PlayModeToString(playMode)
                 ));
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
}
