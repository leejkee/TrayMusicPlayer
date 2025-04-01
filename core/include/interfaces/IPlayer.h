//
// Created by cww on 25-3-31.
//
#pragma once
#include <QObject>
#include <QUrl>


namespace Core::Player {
    class IPlayer : public QObject {
        Q_OBJECT

    public:
        explicit IPlayer(QObject *parent = nullptr) : QObject(parent) {
        }

        enum class PlayMode {
            Sequential,
            LoopOne,
            LoopAll,
            Shuffle
        };

        Q_ENUM(PlayMode);

        // 播放控制
        virtual void play() = 0;

        virtual void pause() = 0;

        virtual void play(const QString &filePath) = 0;

        virtual void next() = 0;

        virtual void previous() = 0;

        // 播放状态查询
        virtual bool getPlayingStatus() const = 0;

        virtual QString getCurrentSongName() const = 0;

        virtual QStringList getCurrentSongNameOfList() const = 0;

        virtual int currentPosition() const = 0; // 当前播放进度（秒）

        virtual int duration() const = 0;        // 当前歌曲总时长（秒）

        // 播放进度控制 seek
        virtual void setPosition(int seconds) = 0;

        // 音量控制
        virtual void setVolume(float volume) = 0;

        virtual float getVolume() const = 0;

        virtual void setPlayMode(PlayMode mode) = 0;

        virtual PlayMode getPlayMode() const = 0;

        // 设置管理
        virtual void addMusicDirectory(const QString &path) = 0;

        virtual void removeMusicDirectory(const QString &path) = 0;

        virtual QStringList getMusicDirectories() const = 0;

        // 播放列表管理
        virtual void switchPlaylist(const QString &playlistName) = 0;

        virtual QString currentPlaylist() const = 0;

        ~IPlayer() override = 0;

    signals:
        // UI 监听这些信号来更新界面
        void playbackStateChanged(bool isPlaying);

        void songChanged(const QString &songName);

        void playbackProgressChanged(int currentSeconds);

        void durationChanged(int totalSeconds);

        void volumeChanged(float newVolume);

        void playModeChanged(PlayMode newMode);

        void playlistSwitched(const QString &newPlaylist);
    };
}
