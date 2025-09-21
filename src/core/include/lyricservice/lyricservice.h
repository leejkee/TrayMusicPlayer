//
// Created by 31305 on 2025/7/21.
//
#pragma once
#include <memory>
#include <QObject>

namespace Tray::Core
{
class LyricServicePrivate;
class LyricService final : public QObject
{
    Q_OBJECT
public:
    explicit LyricService(QObject *parent = nullptr);

    ~LyricService() override;

    /// Updates the lyrics (LRC) for the currently playing music track.
    /// This function is triggered when the music changes.
    /// @param musicName The file name of the MP3 file that will be played.
    void updateLRC(const QString& musicName);

Q_SIGNALS:
    void signalTimingUpdated(int index);

public Q_SLOTS:
    void handlePlayerPositionChange(qint64 pos);

private:
    std::unique_ptr<LyricServicePrivate> d;
    static bool findLRC(const QString& musicPath, QString& lrcPath);
};
}
