//
// Created by cww on 25-4-2.
//
#include <playerbackend/playerbackend.h>
#include <log/log.h>
#include <playlist/playlist.h>
#include <QMediaPlayer>
#include <QAudioOutput>

namespace Tray::Core
{
PlayerBackend::PlayerBackend(QObject* parent)
    : QObject(parent)
{
    m_mediaPlayer = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_playlist = new Playlist(this);
    m_mediaPlayer->setAudioOutput(m_audioOutput);

    connect(m_mediaPlayer
            , &QMediaPlayer::positionChanged
            , this
            , [this](const qint64 pos)
            {
                Q_EMIT signalPositionChanged(pos);
            });

    connect(m_mediaPlayer
            , &QMediaPlayer::mediaStatusChanged
            , this
            , [this](const QMediaPlayer::MediaStatus state)
            {
                if (state == QMediaPlayer::EndOfMedia)
                {
                    Q_EMIT signalMusicOver();
                }
            });

    connect(m_playlist
            , &Playlist::signalPlayModeChanged
            , this
            , [this](const int mode)
            {
                Q_EMIT signalPlayModeChanged(mode);
            });

    connect(m_playlist
            , &Playlist::signalNotifyUiCurrentPlaylistKeyChanged
            , this
            , [this](const QString& key)
            {
                Q_EMIT signalCurrentPlaylistKeyChanged(key);
            });

    connect(m_mediaPlayer
            , &QMediaPlayer::durationChanged
            , this
            , [this](const qint64 duration)
            {
                Q_EMIT signalDurationChanged(duration);
            });
}

void PlayerBackend::setMusicSource(const QString& filePath) const
{
    const auto url = QUrl::fromLocalFile(filePath);
    m_mediaPlayer->setSource(url);
    LOG_INFO(QString("set song url to %1").arg(url.toString()));
}

void PlayerBackend::setVolume(const float v) const
{
    m_audioOutput->setVolume(v);
    Q_EMIT signalVolumeChanged(m_audioOutput->volume());
}

void PlayerBackend::playTg()
{
    if (m_mediaPlayer->isPlaying())
    {
        pause();
    }
    else
    {
        play();
    }
}

void PlayerBackend::pause()
{
    if (m_mediaPlayer->isPlaying())
    {
        m_mediaPlayer->pause();
    }
    LOG_INFO("Player state changed to [Paused]");
    Q_EMIT signalPlayingChanged(false);
}

void PlayerBackend::play()
{
    if (!m_mediaPlayer->isPlaying())
    {
        m_mediaPlayer->play();
    }
    LOG_INFO("Player state changed to [Playing]");
    Q_EMIT signalPlayingChanged(true);
}


void PlayerBackend::setMusicPosition(const qint64 position)
{
    m_mediaPlayer->setPosition(position);
}

bool PlayerBackend::isPlaying() const
{
    return m_mediaPlayer->isPlaying();
}

void PlayerBackend::muteTg()
{
    const bool b = m_audioOutput->isMuted();
    m_audioOutput->setMuted(!b);
    LOG_INFO(QString("set mute to %1").arg(!b));
    Q_EMIT signalMuteChanged(!b);
}

void PlayerBackend::muteOn()
{
    m_audioOutput->setMuted(true);
    LOG_INFO("set mute to ON");
    Q_EMIT signalMuteChanged(true);
}

void PlayerBackend::muteOff()
{
    m_audioOutput->setMuted(false);
    LOG_INFO("set mute to OFF");
    Q_EMIT signalMuteChanged(false);
}

void PlayerBackend::nextMusic()
{
    changeMusic(m_playlist->nextMusic());
}

void PlayerBackend::preMusic()
{
    changeMusic(m_playlist->preMusic());
}

void PlayerBackend::changeMusic(const qsizetype index)
{
    const auto music = m_playlist->getMusic(index);
    setMusicSource(music.m_path);
    Q_EMIT signalCurrentMusicChanged(index, music.m_title, music.m_path);
}

void PlayerBackend::handlePlaylistCacheChanged(const QString& key
                                               , std::shared_ptr<QList<
                                                   Tray::Core::MusicMetaData>>
                                               list)
{
    if (key == m_playlist->getListKey())
    {
        m_playlist->loadPlaylist(key, list);
    }
}

void PlayerBackend::handlePlaylistItemSelection(const QString& listKey
                                                , std::shared_ptr<QList<
                                                    Tray::Core::MusicMetaData>>
                                                list
                                                , const int index)
{
    // check playlist ?
    if (m_playlist->getListKey() != listKey)
    {
        // yes, switch to new playlist
        LOG_INFO("Current list is not [" + listKey + "], switch to it");
        m_playlist->loadPlaylist(listKey, list);
    }
    changeMusic(index);
}

void PlayerBackend::changePlayMode()
{
    m_playlist->changePlayMode();
}
}
