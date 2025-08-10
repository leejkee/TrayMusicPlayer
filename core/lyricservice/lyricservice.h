//
// Created by 31305 on 2025/7/21.
//
#pragma once
#include <song.h>
#include <QString>
#include <memory>
namespace Tray::Core
{

class LyricServicePrivate;
class LyricService
{
public:
    LyricService();

    void loadLRC(const QString &lrcPath);

Q_SIGNALS:
    void signalUpdateLRCToUi(const QStringList &lrc);

public Q_SLOTS:

    void handleLRCToUi(const Song &song);

private:
    std::unique_ptr<LyricServicePrivate> d;

    static bool findLRC(const QString& musicPath, QString &lrcPath);
};
}
