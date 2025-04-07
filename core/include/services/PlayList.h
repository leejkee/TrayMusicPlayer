//
// Created by cww on 25-4-6.
//
#pragma once
#include <Song.h>
#include <Logger_qt.h>
#include <QObject>
#include <QVector>


namespace Core::Service {
    class PlayList final : public QObject {
        Q_OBJECT

    public:
        explicit PlayList(const QVector<Song> &musicList, QObject *parent = nullptr);

        [[nodiscard]] QString getCurrentMusicPath() const;

        void loadMusicList(const QVector<Song> &musicList);

        void setCurrentMusicIndex(unsigned index);

        [[nodiscard]] unsigned getCurrentMusicIndex() const;

    Q_SIGNALS:
        void signalMusicNameChanged(const QString &);

        void signalMusicIndexChanged(unsigned);

    private:
        unsigned m_index;
        QVector<Song> m_musicList;
        Logger_QT Log;
    };
}
