//
// Created by cww on 25-3-31.
//
#pragma once
#include <Logger_qt.h>
#include <QObject>


class QMediaPlayer;
class QAudioOutput;


namespace Core::Engine {
    class Player final : public QObject {
        Q_OBJECT
    public:
        explicit Player(QObject *parent = nullptr);

        void setMusicSource(const QString &source) const;

        void setVolume(float) const;

        void playTg();

        [[nodiscard]] bool isPlaying() const;

    Q_SIGNALS:
        void signalPlayingChanged(bool b);

        void signalIsMuted(bool);

    private:
        QMediaPlayer *m_player;
        QAudioOutput *m_output;
        Service::Logger_QT Log;
    };
}
