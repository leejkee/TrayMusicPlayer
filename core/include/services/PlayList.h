//
// Created by cww on 25-4-6.
//
#pragma once
#include <Song.h>
#include <PlayMode.h>
#include <Logger_qt.h>
#include <QObject>
#include <QVector>


namespace Core::Service {
    class PlayList final : public QObject {
        Q_OBJECT

    public:
        explicit PlayList(const QVector<Song> &musicList, QObject *parent = nullptr);

        [[nodiscard]] QString getCurrentMusicPath() const;

        [[nodiscard]] QString getCurrentMusicTitle() const;

        void loadMusicList(const QVector<Song> &musicList);

        void nextMusic();

        void preMusic();

        void setCurrentMusicIndex(qsizetype index);

        void setPlayMode(PlayMode playMode);

        [[nodiscard]] qsizetype getCurrentMusicIndex() const;

        void changePlayMode();

    Q_SIGNALS:
        void signalMusicNameChanged(const QString &);

        void signalMusicIndexChanged(qsizetype);

        void signalMusicDurationChanged(int);

        void signalMusicChanged(qsizetype, const QString &, int);

        void signalPlayModeChanged(PlayMode);


    private:
        qsizetype m_index;
        PlayMode m_playMode;
        QVector<Song> m_musicList;
        Logger_QT Log;

        [[nodiscard]] qsizetype getNextMusicIndex() const;

        [[nodiscard]] qsizetype getPreMusicIndex() const;
    };
}
