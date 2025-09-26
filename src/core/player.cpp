//
// Created by cww on 25-4-2.
//
#include <player/player.h>
#include <log/log.h>
#include <QMediaPlayer>
#include <QAudioOutput>


namespace Tray::Core
{

Player::Player(QObject* parent)
    : QObject(parent)
{
    setObjectName(QStringLiteral("Player"));
    m_player = new QMediaPlayer(this);
    m_output = new QAudioOutput(this);
    m_player->setAudioOutput(m_output);

    connect(m_player
            , &QMediaPlayer::positionChanged
            , this
            , [this](const qint64 pos)
            {
                Q_EMIT signalPositionChanged(pos);
            });

    connect(m_player
            , &QMediaPlayer::mediaStatusChanged
            , this
            , [this](const QMediaPlayer::MediaStatus state)
            {
                if (state == QMediaPlayer::EndOfMedia)
                {
                    Q_EMIT signalMusicOver();
                }
            });
    LOG_INFO(QString("Player initialized successfully"));
}

void Player::setMusicSource(const QString& source) const
{
    const auto url = QUrl::fromLocalFile(source);
    m_player->setSource(url);
    LOG_INFO(QString("set song url to %1").arg(url.toString()));
}

void Player::setVolume(const float v) const
{
    m_output->setVolume(v);
    Q_EMIT signalVolumeChanged(m_output->volume());
}

void Player::playTg()
{
    const bool isPlaying = m_player->isPlaying();
    if (isPlaying)
    {
        m_player->pause();
    }
    else
    {
        m_player->play();
    }
    LOG_INFO(QString("playing status changed: %1").arg(!isPlaying));
    Q_EMIT signalPlayingChanged(!isPlaying);
}

void Player::pause()
{
    if (m_player->isPlaying())
    {
        m_player->pause();
    }
    LOG_INFO("Player state changed to [Paused]");
    Q_EMIT signalPlayingChanged(false);
}

void Player::play()
{
    if (!m_player->isPlaying())
    {
        m_player->play();
    }
    LOG_INFO("Player state changed to [Playing]");
    Q_EMIT signalPlayingChanged(true);
}


void Player::setMusicPosition(const qint64 position)
{
    m_player->setPosition(position);
}

bool Player::isPlaying() const
{
    return m_player->isPlaying();
}

void Player::muteTg()
{
    const bool b = m_output->isMuted();
    m_output->setMuted(!b);
    LOG_INFO(QString("set mute to %1").arg(!b));
    Q_EMIT signalMuteChanged(!b);
}

void Player::muteOn()
{
    m_output->setMuted(true);
    LOG_INFO("set mute to ON");
    Q_EMIT signalMuteChanged(true);
}

void Player::muteOff()
{
    m_output->setMuted(false);
    LOG_INFO("set mute to OFF");
    Q_EMIT signalMuteChanged(false);
}

}
