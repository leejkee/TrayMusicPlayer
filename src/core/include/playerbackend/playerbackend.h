//
// Created by cww on 25-3-31.
//
#pragma once
#include <musicmetadata.h>
#include <QObject>

class QMediaPlayer;
class QAudioOutput;

namespace Tray::Core
{
class Playlist;

class PlayerBackend final : public QObject
{
    Q_OBJECT

public:
    explicit PlayerBackend(QObject* parent = nullptr);

    void setMusicSource(const QString& filePath) const;

    void setVolume(float) const;

    void playTg();

    void play();

    void pause();

    void setMusicPosition(qint64 position);

    [[nodiscard]] bool isPlaying() const;

    void muteTg();

    void muteOn();

    void muteOff();

    void changePlayMode();

Q_SIGNALS:
    void signalPlayingChanged(bool b);

    void signalPositionChanged(qint64 position);

    void signalMusicOver();

    void signalMuteChanged(bool);

    void signalVolumeChanged(float x) const;

    void signalCurrentMusicChanged(qint64 index
                                   , const QString& musicTitle
                                   , const QString& musicPath);

    void signalPlayModeChanged(int mode);

    void signalCurrentPlaylistKeyChanged(const QString& key);

    void signalDurationChanged(qint64 duration);

public Q_SLOTS:
    void nextMusic();

    void preMusic();

    void changeMusic(qsizetype index);

    void handlePlaylistCacheChanged(const QString& key
                                    , std::shared_ptr<QList<Tray::Core::
                                        MusicMetaData>> list);
    void handlePlaylistItemSelection(const QString& listKey
                                     , std::shared_ptr<QList<
                                         Tray::Core::MusicMetaData>> list
                                     , const int index);

private:
    QMediaPlayer* m_mediaPlayer;

    QAudioOutput* m_audioOutput;

    Playlist* m_playlist;
};
}
