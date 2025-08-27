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

void Player::setMusicPosition(const qint64 position)
{
    m_player->setPosition(position);
}

bool Player::isPlaying() const
{
    return m_player->isPlaying();
}

void Player::setMute()
{
    const bool b = m_output->isMuted();
    m_output->setMuted(!b);
    LOG_INFO(QString("set mute to %1").arg(!b));
    Q_EMIT signalMuteChanged(!b);
}
}
