//
// Created by cww on 25-4-6.
//
#pragma once
#include <musicmetadata.h>
#include <QObject>
#include <QVector>
#include <memory>

namespace Tray::Core
{
class PlaylistPrivate;
class Playlist final : public QObject
{
    Q_OBJECT
public:
    explicit Playlist(QObject* parent = nullptr);

    enum class PlayMode
    {
        Sequential = 0, LoopOne = 1, LoopAll = 2, Shuffle = 3
    };

    static QString PlayModeToString(const PlayMode& mode)
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

    void loadPlaylist(const QString& listKey
                       , const QList<MusicMetaData>& playlist);

    void nextMusic();

    void preMusic();

    [[nodiscard]] QString getCurrentMusicPath() const;

    [[nodiscard]] qsizetype getCurrentMusicIndex() const;

    [[nodiscard]] QString getListKey() const;

    void setCurrentMusicIndex(qsizetype index);

    void changePlayMode();

    [[nodiscard]] MusicMetaData currentMusic() const;

Q_SIGNALS:
    void signalCurrentMusicChanged(int index, const Tray::Core::MusicMetaData& music);

    void signalPlayModeChanged(int);

    void signalNotifyUiCurrentPlaylistKeyChanged(const QString& key);

public Q_SLOTS:
    void handleCurrentListChanged(const QString& listKey
                           , const QList<Tray::Core::MusicMetaData>& playlist);

private:
    std::unique_ptr<PlaylistPrivate> d;

    [[nodiscard]] qsizetype getNextMusicIndex() const;

    [[nodiscard]] qsizetype getPreMusicIndex() const;

    void setPlayMode(PlayMode playMode);
};
}
