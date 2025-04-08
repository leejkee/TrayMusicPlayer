//
// Created by cww on 25-4-2.
//
#pragma once
#include <QObject>
#include <QString>


namespace Core {
    class ICore : public QObject {
        Q_OBJECT

    public:
        explicit ICore(QObject *parent = nullptr)
            : QObject(parent) {
        }

        static ICore *create(QObject *parent = nullptr);

        enum class PlayMode {
            Sequential,
            LoopOne,
            LoopAll,
            Shuffle
        };
        // init the default settings
        virtual void initDefaultSettings() = 0;

    Q_SIGNALS:
        // emitted when the QMediaPlayer::positionChanged() emitted
        void signalPositionChanged(qint64 position);

        void signalCurrentMusicNameChanged(const QString &musicName);

        void signalPlayingChanged(bool);

        void signalIsMuted(bool);

    public Q_SLOTS:
        virtual void playToggle() = 0;

        virtual void nextMusic() = 0;

        virtual void preMusic() = 0;

        virtual void setVolume(unsigned int) = 0;

        virtual void loadMusic(const QString &musicPath) = 0;

        virtual void switchMusicListByName(const QString &listName) = 0;

        virtual QString getDefaultMusicName() = 0;
    };
} // namespace Core
