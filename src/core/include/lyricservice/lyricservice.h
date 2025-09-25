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
    /// @param musicPath The path of the MP3 file that will be played.
    void updateLRC(const QString& musicPath);

    [[nodiscard]] QList<int64_t> lrcTiming() const;

    [[nodiscard]] QStringList lrcText() const;

Q_SIGNALS:
    void signalTimingUpdated(int index);

public Q_SLOTS:
    void handlePlayerPositionChange(qint64 pos);

private:
    std::unique_ptr<LyricServicePrivate> d;
    static bool findLRC(const QString& musicPath, QString& lrcPath);
    static int findPositionIndex(const QList<int64_t>& lrcTiming, int64_t value);
};
}
