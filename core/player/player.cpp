//
// Created by cww on 25-4-2.
//
#include "player.h"
#include "QLogger.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <qtmetamacros.h>


namespace Tray::Core {
    Player::Player(QObject *parent) : QObject(parent) {
        setObjectName(QStringLiteral("Player"));
        Log = Log::QLogger(this->objectName());
        m_player = new QMediaPlayer(this);
        m_output = new QAudioOutput(this);
        m_player->setAudioOutput(m_output);

        connect(m_player, &QMediaPlayer::positionChanged, this, [this](const qint64 pos) {
            Q_EMIT signalPositionChanged(pos);
        });

        connect(m_player, &QMediaPlayer::mediaStatusChanged, this, [this](const QMediaPlayer::MediaStatus state) {
           if (state == QMediaPlayer::EndOfMedia) {
               Q_EMIT signalMusicOver();
           }
        });
        Log.log(Log::QLogger::LogLevel::Info, "Player initialized successfully");
    }

    void Player::setMusicSource(const QString &source) const {
        const auto url = QUrl::fromLocalFile(source);
        m_player->setSource(url);
        Log.log(Log::QLogger::LogLevel::Info, "set song url to " + url.toString());
    }

    void Player::setVolume(const float v) const {
        m_output->setVolume(v);
    }

    void Player::playTg() {
        const bool isPlaying = m_player->isPlaying();
        if (isPlaying) {
            m_player->pause();
        } else {
            m_player->play();
        }
        Log.log(Log::QLogger::LogLevel::Info, "playing status changed: " + Log::QLogger::boolToString(!isPlaying));
        Q_EMIT signalPlayingChanged(!isPlaying);
    }

    void Player::setMusicPosition(const qint64 position) {
        m_player->setPosition(position);
    }

    bool Player::isPlaying() const {
        return m_player->isPlaying();
    }

    void Player::setMute(){
        bool b = m_output->isMuted();
        m_output->setMuted(!b);
        Log.log(Log::QLogger::LogLevel::Info, "set mute to " + Log::QLogger::boolToString(!b));
        Q_EMIT signalMuteChanged(!b);
    }
}
