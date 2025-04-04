//
// Created by cww on 25-4-2.
//
#include <Player.h>
#include <QMediaPlayer>
#include <QAudioOutput>


namespace Core::Engine {

    Player::Player(QObject *parent) : QObject(parent) {
        m_player = new QMediaPlayer(this);
        m_output = new QAudioOutput(this);
        m_player->setAudioOutput(m_output);
    }

    void Player::setMusicSource(const QString &source) const {
        auto url = QUrl::fromLocalFile(source);
        m_player->setSource(url);
    }

    void Player::setVolume(const float v) const {
        m_output->setVolume(v);
    }

    void Player::playTg() const {
        if (m_player->isPlaying()) {
            m_player->pause();
        }
        else {
            m_player->play();
        }
    }

    bool Player::isPlaying() const {
        return m_player->isPlaying();
    }
}
