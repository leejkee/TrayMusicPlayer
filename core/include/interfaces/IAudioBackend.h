//
// Created by cww on 25-3-31.
//
#pragma once
#include <QObject>
#include <QUrl>

class IAudioBackend : public QObject {
    Q_OBJECT

public:
    enum State { StoppedState, PlayingState, PausedState };

    Q_ENUM(State)


    virtual ~IAudioBackend() = default;

    // basic control
    virtual void load(const QUrl &url) = 0;

    virtual void play() = 0;

    virtual void pause() = 0;

    virtual void stop() = 0;

    // state of player
    [[nodiscard]] virtual State state() const = 0;

    [[nodiscard]] virtual qint64 position() const = 0;

    [[nodiscard]] virtual qint64 duration() const = 0;

    virtual void setPosition(qint64 pos) = 0;

    // controller of volume
    [[nodiscard]] virtual float volume() const = 0;

    virtual void setVolume(float volume) = 0;

signals:
    void signalStateChanged(State newState);

    void signalPositionChanged(qint64 position);

    void signalDurationChanged(qint64 duration);

    void signalVolumeChanged(float volume);

    void signalErrorOccurred(const QString &message);
};
