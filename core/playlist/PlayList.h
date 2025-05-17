//
// Created by cww on 25-4-6.
//
#pragma once
#include <Song.h>
#include <QLogger.h>
#include <QObject>
#include <QVector>


namespace Tray::Core {
    class PlayList final : public QObject {
        Q_OBJECT

    public:
        explicit PlayList(QObject *parent);

        enum class PlayMode {
            Sequential = 0,
            LoopOne    = 1,
            LoopAll    = 2,
            Shuffle    = 3
        };

        static QString PlayModeToString(const PlayMode mode) {
            switch (mode) {
                case PlayMode::Sequential:
                    return QStringLiteral("Sequential");
                case PlayMode::LoopOne:
                    return QStringLiteral("LoopOne");
                case PlayMode::LoopAll:
                    return QStringLiteral("LoopAll");
                case PlayMode::Shuffle:
                    return QStringLiteral("Shuffle");
                default:
                    return {};
            }
        }

        [[nodiscard]] QString getCurrentMusicPath() const;


        void loadMusicList(const QString &listKey, const QVector<Song> &musicList);

        void nextMusic();

        void preMusic();

        void setCurrentMusicIndex(qsizetype index);

        [[nodiscard]] qsizetype getCurrentMusicIndex() const;

        [[nodiscard]] QString getListKey() const;

        void changePlayMode();

    Q_SIGNALS:
        void signalMusicChanged(qsizetype index, const QString &title, int duration);

        void signalPlayModeChanged(int);

        void signalCurrentPlaylistKeyChanged(const QString &key);
    private:
        qsizetype m_index;
        PlayMode m_playMode;
        QVector<Song> m_musicList;
        QString m_currentListKey;
        Log::QLogger Log;

        [[nodiscard]] qsizetype getNextMusicIndex() const;

        [[nodiscard]] qsizetype getPreMusicIndex() const;

        void setPlayMode(PlayMode playMode);
    };
}
