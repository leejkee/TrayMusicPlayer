//
// Created by 31305 on 2025/7/21.
//
#include "lyricservice.h"
#include <lyricparser.h>
#include <QFile>
#include <song.h>

namespace Tray::Core
{
class LyricServicePrivate
{
public:
    Badfish::AudioToolkit::LyricParser m_parser;
};

LyricService::LyricService() : d(std::make_unique<LyricServicePrivate>())
{
}

void LyricService::loadLRC(const QString& lrcPath)
{
    d->m_parser.clear_result();
    d->m_parser.load_file(lrcPath.toStdString());
}

void LyricService::handleLRCToUi(const Song &song)
{
    QString lrcPath;
    findLRC(song.m_path, lrcPath);
    loadLRC(lrcPath);
}

bool LyricService::findLRC(const QString& musicPath, QString &lrcPath)
{
    if (const auto indexSuffix = musicPath.lastIndexOf('.'); indexSuffix != -1)
    {
        const auto LRC_SUFFIX = "lrc";
        lrcPath = musicPath.left(indexSuffix + 1) + LRC_SUFFIX;
    }
    else
    {
        return false;
    }
    if (const QFile file(lrcPath); file.exists())
    {
        return true;
    }
    return false;
}

}
