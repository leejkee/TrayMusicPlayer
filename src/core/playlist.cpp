//
// Created by cww on 25-4-6.
//
#include <playlist/playlist.h>
#include <log/log.h>
#include <QRandomGenerator>

namespace Tray::Core
{
class PlaylistPrivate
{
public:
    qsizetype m_index;
    Playlist::PlayMode m_playMode;
    QList<MusicMetaData> m_musicList;
    QString m_currentListKey;
};

Playlist::Playlist(QObject* parent)
    : QObject(parent), d(std::make_unique<PlaylistPrivate>())
{
    d->m_index = -1;
    d->m_playMode = PlayMode::Sequential;
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
    d->m_currentListKey = listKey;
    d->m_musicList = playlist;
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
    switch (d->m_playMode)
    {
    case PlayMode::Sequential:
    case PlayMode::LoopAll:
        return (d->m_index + 1) % d->m_musicList.size();
    case PlayMode::LoopOne:
        return d->m_index;
    case PlayMode::Shuffle:
        return QRandomGenerator::global()->bounded(d->m_musicList.size());
    default:
        return 0;
    }
}

qsizetype Playlist::getPreMusicIndex() const
{
    switch (d->m_playMode)
    {
    case PlayMode::Sequential:
    case PlayMode::LoopAll:
        return (d->m_index == 0)
               ? d->m_musicList.size() - 1
               : d->m_index - 1;
    case PlayMode::LoopOne:
        return d->m_index;
    case PlayMode::Shuffle:
        return QRandomGenerator::global()->bounded(d->m_musicList.size());
    default:
        return 0;
    }
}

QString Playlist::getCurrentMusicPath() const
{
    return d->m_musicList.at(d->m_index).m_path;
}

qsizetype Playlist::getCurrentMusicIndex() const
{
    return d->m_index;
}

void Playlist::setCurrentMusicIndex(const qsizetype index)
{
    if (index >= d->m_musicList.size())
    {
        LOG_ERROR("index out of range");
    }
    d->m_index = index;
    LOG_INFO(QString("index changed: %1").arg(QString::number(d->m_index)));
    Q_EMIT signalCurrentMusicChanged(static_cast<int>(d->m_index), d->m_musicList.at(d->m_index));
}

void Playlist::setPlayMode(const PlayMode playMode)
{
    if (d->m_playMode != playMode)
    {
        d->m_playMode = playMode;
        Q_EMIT signalPlayModeChanged(static_cast<int>(playMode));
        LOG_INFO(QString("play mode changed: %1").arg(PlayModeToString(playMode)));
    }
}

void Playlist::changePlayMode()
{
    switch (d->m_playMode)
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
    return d->m_currentListKey;
}

void Playlist::handleCurrentListChanged(const QString& listKey, const QList<MusicMetaData>& playlist)
{
    if (d->m_currentListKey == listKey)
    {
        loadPlaylist(listKey, playlist);
    }
}

MusicMetaData Playlist::currentMusic() const
{
    return d->m_musicList.at(d->m_index);
}

}
