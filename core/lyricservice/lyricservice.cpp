//
// Created by 31305 on 2025/7/21.
//
#include "lyricservice.h"
#include <lyricparser.h>
#include <QFile>

namespace Tray::Core
{
class LyricServicePrivate
{
public:
    Badfish::AudioToolkit::LyricParser m_parser;
};

LyricService::LyricService()
    : d(std::make_unique<LyricServicePrivate>())
{
}

void LyricService::loadLRC(const QString& lrcPath)
{
    d->m_parser.clear_result();
    d->m_parser.load_file(lrcPath.toStdString());
}

void LyricService::updateLRC(const QString& musicName)
{
    QStringList lrcText;
    QList<int64_t> lrcTiming;
    if (QString lrcPath; findLRC(musicName, lrcPath))
    {
        loadLRC(lrcPath);
        for (const auto& lyric : d->m_parser.get_lrc())
        {
            lrcText.append(QString::fromStdString(lyric.m_text));
            if (lyric.isTag())
            {
                lrcTiming.append(-1);
            }
            else
            {
                lrcTiming.append(lyric.m_start_ms.value());
            }
        }
    }
    Q_EMIT signalUpdateLRCToModel(musicName, lrcText, lrcTiming);
}

bool LyricService::findLRC(const QString& musicPath, QString& lrcPath)
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
