//
// Created by cww on 25-4-2.
//
#pragma once
#include <PlayMode.h>
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

        /// Init the default settings.
        /// This function will init some objects to make ui looks great,
        /// and make it can be used directly
        virtual void initDefaultSettings() = 0;

    Q_SIGNALS:
        // emitted when the QMediaPlayer::positionChanged() emitted
        void signalPositionChanged(qint64 position);

        void signalCurrentMusicNameChanged(const QString &musicName);

        void signalCurrentMusicIndexChanged(qsizetype index);

        void signalCurrentMusicDurationChanged(int seconds);

        void signalCurrentMusicChanged(int, const QString &, int);

        void signalPlayingChanged(bool);

        void signalIsMuted(bool);

        void signalPlayModeChanged(int);

        void signalMusicListChanged(const QString &name, const QStringList &nameList);

    public Q_SLOTS:
        /// Toggles between play and pause states of the current music.
        virtual void playToggle() = 0;

        /// Skips to the next track in the current playlist.
        virtual void nextMusic() = 0;

        /// Returns to the previous track in the current playlist.
        virtual void preMusic() = 0;

        /// Plays the music with index.
        virtual void playToggleIndex(int) = 0;

        /// Adjusts the playback volume (0-100 range expected).
        virtual void setVolume(unsigned int volume) = 0;

        /// Loads and prepares a music file from the given path for playback.
        virtual void loadMusic(const QString &musicPath) = 0;

        /// Sets the playback mode (e.g., Repeat, Shuffle, Sequential).
        virtual void setPlayMode(const Service::PlayMode &mode) = 0;

        /// Switches the active playlist to the specified list name.
        virtual void switchMusicListByName(const QString &listName) = 0;

        /// todo: not interface?
        /// Retrieves the music titles belonging to the specified playlist.
        /// @return QStringList of music titles, empty if playlist doesn't exist.
        virtual QStringList getMusicListByName(const QString &name) = 0;

        /// Handles UI request to fetch music list by name.
        /// Triggers a signal with the corresponding QStringList of music titles.
        /// If no cached data exists for the given name, an empty list is emitted.
        virtual void requestMusicListByName(const QString &name) = 0;

        /// Seeks the current track to the specified position (in milliseconds).
        virtual void setMusicPosition(qint64 position) = 0;

        /// Cycles to the next available playback mode (e.g., Sequential -> LoopAll -> LoopOne -> Shuffle
        /// -> Sequential ... ).
        virtual void changePlayMode() = 0;

        /// Gets all keys of user playlist, which can be used to create the ui components
        /// UI can also request fetching music list by these keys
        /// @return QStringList of all keys of user playlist
        virtual QStringList getKeysUserList() = 0;
    };
} // namespace Core
