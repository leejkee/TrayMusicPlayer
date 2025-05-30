//
// Created by cww on 25-3-31.
//
#pragma once
#include <QLogger.h>
#include <QObject>


class QMediaPlayer;
class QAudioOutput;


namespace Tray::Core {
    class Player final : public QObject {
        Q_OBJECT
    public:
        explicit Player(QObject *parent = nullptr);

        void setMusicSource(const QString &source) const;

        void setVolume(float) const;

        void playTg();

        void setMusicPosition(qint64 position);

        [[nodiscard]] bool isPlaying() const;

        void setMute();

    Q_SIGNALS:
        void signalPlayingChanged(bool b);

        void signalPositionChanged(qint64 position);

        void signalMusicOver();

        void signalMuteChanged(bool);

    private:
        QMediaPlayer *m_player;
        QAudioOutput *m_output;
        Log::QLogger Log;
    };
}
