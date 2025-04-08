//
// Created by cww on 25-4-2.
//
#include <Player.h>
#include <QMediaPlayer>
#include <QAudioOutput>


namespace Core::Engine {
    Player::Player(QObject *parent) : QObject(parent) {
        setObjectName(QStringLiteral("Player"));
        Log = Service::Logger_QT(this->objectName());
        m_player = new QMediaPlayer(this);
        m_output = new QAudioOutput(this);
        m_player->setAudioOutput(m_output);
        connect(m_output, &QAudioOutput::volumeChanged, this, [this](const float v) {
            Log.log(Service::Logger_QT::LogLevel::Info, "signal emitted, volume changed: " + QString::number(v));
            Q_EMIT signalIsMuted(v == 0);
        });
    }

    void Player::setMusicSource(const QString &source) const {
        const auto url = QUrl::fromLocalFile(source);
        m_player->setSource(url);
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
        Log.log(Service::Logger_QT::LogLevel::Info, "playing status changed: " + QString::number(!isPlaying));
        Q_EMIT signalPlayingChanged(!isPlaying);
    }

    bool Player::isPlaying() const {
        return m_player->isPlaying();
    }
}
