//
// Created by cww on 25-4-6.
//
#pragma once
#include <musicmetadata.h>
#include <QObject>
#include <QVector>


namespace Tray::Core
{
class Playlist final : public QObject
{
    Q_OBJECT

public:

    explicit Playlist(QObject* parent = nullptr);

    enum class PlayMode
    {
        Sequential = 0, LoopOne = 1, LoopAll = 2, Shuffle = 3
    };

    static QString PlayModeToString(const PlayMode mode)
    {
        switch (mode)
        {
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

    void loadPlaylist(const QString& listKey
                       , const QList<MusicMetaData>& playlist);

    void nextMusic();

    void preMusic();

    void setCurrentMusicIndex(qsizetype index);

    [[nodiscard]] qsizetype getCurrentMusicIndex() const;

    [[nodiscard]] QString getListKey() const;

    void changePlayMode();

Q_SIGNALS:
    void signalCurrentMusicChanged(qsizetype index
                                   , const QString& title
                                   , int duration);

    void signalPlayModeChanged(int);

    void signalNotifyUiCurrentPlaylistKeyChanged(const QString& key);

private:
    qsizetype m_index;
    PlayMode m_playMode;
    QList<MusicMetaData> m_musicList;
    QString m_currentListKey;

    [[nodiscard]] qsizetype getNextMusicIndex() const;

    [[nodiscard]] qsizetype getPreMusicIndex() const;

    void setPlayMode(PlayMode playMode);
};
}
