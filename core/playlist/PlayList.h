//
// Created by cww on 25-4-6.
//
#pragma once
#include "../Song.h"
#include "../PlayMode.h"
#include "../QLogger.h"
#include <QObject>
#include <QVector>


namespace Tray::Core {
    class PlayList final : public QObject {
        Q_OBJECT

    public:
        explicit PlayList(QObject *parent);

        [[nodiscard]] QString getCurrentMusicPath() const;

        void loadMusicList(const QString &listKey, const QVector<Song> &musicList);

        void nextMusic();

        void preMusic();

        void setCurrentMusicIndex(qsizetype index);

        [[nodiscard]] qsizetype getCurrentMusicIndex() const;

        QString getListKey() const;

        void changePlayMode();

    Q_SIGNALS:
        void signalMusicChanged(qsizetype index, const QString &title, int duration);

        void signalPlayModeChanged(PlayMode);

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
