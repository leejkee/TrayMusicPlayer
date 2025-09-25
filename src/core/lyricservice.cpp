//
// Created by 31305 on 2025/7/21.
//
#include <lyricservice/lyricservice.h>
#include <lyricparser.h>
#include <QFile>
#include <algorithm>

namespace Tray::Core
{
class LyricServicePrivate
{
public:
    QStringList m_lyrics;
    QList<int64_t> m_lrcTiming;
    int m_lyricLineIndex{0};
};

LyricService::LyricService(QObject* parent)
    : QObject(parent),
      d(std::make_unique<LyricServicePrivate>())
{
}

LyricService::~LyricService() = default;

void LyricService::updateLRC(const QString& musicPath)
{
    d->m_lyrics.clear();
    d->m_lrcTiming.clear();
    d->m_lyricLineIndex = 0;

    if (QString lrcPath; findLRC(musicPath, lrcPath))
    {
        Badfish::AudioToolkit::LyricParser lyricParser;
        lyricParser.load_file(lrcPath.toStdString());
        for (const auto& lyric : lyricParser.get_lrc())
        {
            d->m_lyrics.append(QString::fromStdString(lyric.m_text));
            if (lyric.isTag())
            {
                d->m_lrcTiming.append(0);
            }
            else
            {
                d->m_lrcTiming.append(lyric.m_start_ms.value());
            }
        }
    }
}

bool LyricService::findLRC(const QString& musicPath, QString& lrcPath)
{
    if (const auto indexSuffix = musicPath.lastIndexOf('.'); indexSuffix != -1)
    {
        const QString LRC_SUFFIX{"lrc"};
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

void LyricService::handlePlayerPositionChange(const qint64 pos)
{
    if (d->m_lyricLineIndex > d->m_lyrics.length() - 2)
    {
        return;
    }

    if (pos >= d->m_lrcTiming.at(d->m_lyricLineIndex + 1) && pos < d->
        m_lrcTiming.at(d->m_lyricLineIndex + 2))
    {
        d->m_lyricLineIndex++;
    }
    else
    {
        auto findPositionIndex = [](const QList<int64_t>& lrcTiming
                                    , const int64_t value)
        {
            const auto it = std::upper_bound(lrcTiming.begin()
                                             , lrcTiming.end()
                                             , value);
            return static_cast<int>(std::distance(lrcTiming.begin(), it)) - 1;
        };
        d->m_lyricLineIndex = findPositionIndex(d->m_lrcTiming, pos);
    }
    Q_EMIT signalTimingUpdated(d->m_lyricLineIndex);
}


QList<int64_t> LyricService::lrcTiming() const
{
    return d->m_lrcTiming;
}

QStringList LyricService::lrcText() const
{
    return d->m_lyrics;
}
}
