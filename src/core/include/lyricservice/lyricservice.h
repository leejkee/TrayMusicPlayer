//
// Created by 31305 on 2025/7/21.
//
#pragma once
#include <QString>
#include <memory>
#include <QObject>

namespace Tray::Core
{
class LyricServicePrivate;

class LyricService final : QObject
{
    Q_OBJECT
public:
    explicit LyricService(QObject *parent = nullptr);

    ~LyricService();

    void loadLRC(const QString& lrcPath);

Q_SIGNALS:
    void signalUpdateLRCToModel(const QString& musicName, const QStringList& lrcText
                             , const QList<int64_t>& lrcTiming);

public Q_SLOTS:

/// Updates the lyrics (LRC) for the currently playing music track.
/// This function is triggered when the music changes.
/// @param musicName The file name of the MP3 file that will be played.
    void updateLRC(const QString& musicName);

private:
    std::unique_ptr<LyricServicePrivate> d;

    static bool findLRC(const QString& musicPath, QString& lrcPath);
};
}
